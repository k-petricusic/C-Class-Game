#include "GuardMovementStrategies.h"
#include <unordered_map>
#include <functional>
#include <random>

// 1. Random Search Strategy
void RandomSearchStrategy::move(Guard& guard, Board_Screen& board) {
    (void)board; // Prevent unused parameter warning
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 4);
    guard.set_direction(dis(gen));
    board.move(guard, guard.get_direction());
}

// -------------------------------- Move around wall strategy --------------------------------
MoveAroundWallStrategy::MoveAroundWallStrategy(bool wall_side) : wall_side(wall_side) {}

// Helper to get the direction to the right or left
static size_t turn_right(size_t dir) { return dir % 4 + 1; }
static size_t turn_left(size_t dir) { return (dir == 1 ? 4 : dir - 1); }

void MoveAroundWallStrategy::move(Guard& guard, Board_Screen& board) {
    size_t dir = guard.get_direction();
    size_t check_dir = wall_side ? turn_right(dir) : turn_left(dir);
    if (board.move(guard, check_dir)) { // if the guard can move to the side, move to the side
        guard.set_direction(check_dir);
    } else if (board.move(guard, dir)) { // if the guard can't move to the side, move forward
    } else { // if the guard can't move to the side or forward, turn away from the wall and move forward
        guard.set_direction(wall_side ? turn_left(dir) : turn_right(dir));
        board.move(guard, dir);
    }
}

// 2. Factory implementation
// Type of function that returns a pointer to an IGuardMovementStrategy when called
using StrategyFactory = std::function<IGuardMovementStrategy*()>;

static std::unordered_map<std::string, StrategyFactory> strategy_factories = {
    {"random", [](){ return new RandomSearchStrategy(); }},
    {"wall_left", [](){ return new MoveAroundWallStrategy(false); }},
    {"wall_right", [](){ return new MoveAroundWallStrategy(true); }},
};

IGuardMovementStrategy* create_guard_movement_strategy(const std::string& name) {
    auto it = strategy_factories.find(name);
    if (it != strategy_factories.end()) {
        return it->second();
    }
    return nullptr; // or a default strategy
}

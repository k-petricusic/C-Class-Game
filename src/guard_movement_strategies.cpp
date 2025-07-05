#include "GuardMovementStrategies.h"
#include <unordered_map>
#include <functional>

// 1. All strategies directly change the guard's direction only
/*
void RandomSearchStrategy::move(Guard& guard, Board_Screen& board) {
    // Implementation
}
*/

// -------------------------------- Move around wall strategy --------------------------------
MoveAroundWallStrategy::MoveAroundWallStrategy(bool wall_side) : wall_side(wall_side) {}

//finish this
void MoveAroundWallStrategy::move(Guard& guard, Board_Screen& board) {
    board.move(guard, guard.get_direction());
    while (!(board.move(guard, guard.get_direction()))) {
        guard.set_direction((guard.get_direction() % 4) + 1); // turns guard 90 degrees clockwise
    }
    board.move(guard, ((guard.get_direction() + 1) % 4) + 1); //moves guard back in the direction it was moved in before the while loop
}

// 2. Factory implementation
// Type of function that returns a pointer to an IGuardMovementStrategy when called
using StrategyFactory = std::function<IGuardMovementStrategy*()>;

static std::unordered_map<std::string, StrategyFactory> strategy_factories = {
    {"random", [](){ return new RandomSearchStrategy(); }},
    {"wall_left", [](){ return new MoveAroundWallStrategy(true); }},
    {"wall_right", [](){ return new MoveAroundWallStrategy(false); }},
};

IGuardMovementStrategy* create_guard_movement_strategy(const std::string& name) {
    auto it = strategy_factories.find(name);
    if (it != strategy_factories.end()) {
        return it->second();
    }
    return nullptr; // or a default strategy
}

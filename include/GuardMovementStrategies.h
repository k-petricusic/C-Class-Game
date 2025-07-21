#pragma once

#include <string>
#include "Guard.h"
#include "Screen.h"

// 1. Base interface for all strategies
class IGuardMovementStrategy {
public:
    virtual ~IGuardMovementStrategy() = default;
    virtual void move(Guard& guard, Board_Screen& board) = 0;
};

// 2. Strategies

// 3. Random Search Strategy
class RandomSearchStrategy : public IGuardMovementStrategy {
    int _direction_duration = 0;
public:
    void move(Guard& guard, Board_Screen& board) override;
};


// 4. Move Around Wall Strategy
class MoveAroundWallStrategy : public IGuardMovementStrategy {
    bool wall_side;
public:
    MoveAroundWallStrategy(bool wall_side);
    void move(Guard& guard, Board_Screen& board) override;
};

// 5. Factory function
IGuardMovementStrategy* create_guard_movement_strategy(const std::string& name);

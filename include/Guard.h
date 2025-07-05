#pragma once

#include "Movable.h"
#include <vector>

class IGuardMovementStrategy; // forward declaration to avoid circular includes

// --------- Guard class ---------
class Guard : public Movable {
    // Guard is a movable object, so it inherits from Movable
private:
    size_t _guard_direction; // 1: up, 2: right, 3: down, 4: left
    IGuardMovementStrategy* _movement_strategy;

public:

    Guard(size_t x = 0, size_t y = 0, size_t direction = 1, IGuardMovementStrategy* strategy = nullptr)
        : Movable(x, y), _guard_direction(direction), _movement_strategy(strategy) {}
    
    size_t get_direction() const { return _guard_direction; }

    bool set_direction(size_t direction) { 
        if (direction < 1 || direction > 4) return false;
        _guard_direction = direction;
        return true;
    }
    
    void set_movement_strategy(IGuardMovementStrategy* strategy) {
        _movement_strategy = strategy;
    }


};
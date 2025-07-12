#pragma once

#include "Movable.h"
#include <vector>

class IGuardMovementStrategy; // forward declaration to avoid circular includes

// --------- Guard class ---------
class Guard : public Movable {
    // Guard is a movable object, so it inherits from Movable
private:
    size_t _guard_direction; // 1: up, 2: right, 3: down, 4: left
    bool _see_through_walls; //whether or not the guard  an see through walls
    IGuardMovementStrategy* _movement_strategy;

public:

    Guard(size_t x = 0, size_t y = 0, size_t direction = 1, IGuardMovementStrategy* strategy = nullptr, bool see_through_walls = false)
        : Movable(x, y), _guard_direction(direction), _see_through_walls(see_through_walls), _movement_strategy(strategy) {}
    
    size_t get_direction() const { return _guard_direction; }

    bool set_direction(size_t direction) { 
        if (direction < 1 || direction > 4) return false;
        _guard_direction = direction;
        return true;
    }

    bool get_see_through_walls() const { return _see_through_walls; }

    void set_see_through_walls(bool see_through_walls) { _see_through_walls = see_through_walls;}
    
    void set_movement_strategy(IGuardMovementStrategy* strategy) {
        _movement_strategy = strategy;
    }

    IGuardMovementStrategy* get_movement_strategy() const { return _movement_strategy; }

};
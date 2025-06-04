#pragma once

#include "Screen.h"
#include "Movable.h"

// --------- Guard class ---------
class Guard : public Movable {
    // Guard is a movable object, so it inherits from Movable
private:
    size_t _guard_direction; // 1: up, 2: right, 3: down, 4: left

public:

    Guard(size_t x = 0, size_t y = 0, size_t direction = 1)
        : Movable(x, y), _guard_direction(direction) {}
    
    size_t get_direction() const { return _guard_direction; }

    bool set_direction(size_t direction) { 
        if (direction < 1 || direction > 4) return false;
        _guard_direction = direction;
        return true;
    }

    // Moves guard around a wall to the left or right side based on the current board state
    void guard_move_around_wall(const std::vector<std::vector<char>> &board, bool wall_side);
};
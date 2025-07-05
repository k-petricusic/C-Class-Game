#pragma once

#include "Movable.h"
class Board_Screen; // forward declare

class Guard : public Movable {
private:
    size_t _guard_direction;

public:
    Guard(size_t x = 0, size_t y = 0, size_t direction = 1)
        : Movable(x, y), _guard_direction(direction) {}

    size_t get_direction() const { return _guard_direction; }

    bool set_direction(size_t direction) {
        if (direction < 1 || direction > 4) return false;
        _guard_direction = direction;
        return true;
    }

    // 🔧 Only declare this here
    void guard_move_around_wall(Board_Screen& board, bool wall_side);
};

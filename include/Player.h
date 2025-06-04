#pragma once

#include "Screen.h"
#include "Movable.h"

// --------- Player class ---------
class Player : public Movable {
    // Player is a movable object, so it inherits from Movable
public:
    Player(size_t x = 0, size_t y = 0)
        : Movable(x, y) {}

    void player_move();
};
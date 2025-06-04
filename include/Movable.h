#pragma once
#include <cstddef>

class Movable {
public:
    Movable(size_t x = 0, size_t y = 0) : _x(x), _y(y) {}
    virtual ~Movable() = default;

    virtual size_t get_x() const { return _x; }
    virtual size_t get_y() const { return _y; }

    virtual void set_x(size_t x) { _x = x; }
    virtual void set_y(size_t y) { _y = y; }

protected:
    size_t _x = 0;
    size_t _y = 0;
};
// -*- C++ -*-

#ifndef POINT_H_
#define POINT_H_

struct Point
{
    int x, y;

    Point() {}
    Point(int x, int y) : x(x), y(y) {}
};

size_t hash_value(const Point& p);
bool operator==(const Point& p1, const Point& p2);
bool operator!=(const Point& p1, const Point& p2);

#include "point-inl.h"

#endif // POINT_H_

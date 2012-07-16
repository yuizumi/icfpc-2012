// -*- C++ -*-

#include <boost/functional/hash.hpp>

inline size_t hash_value(const Point& p)
{
    size_t h = 0;
    boost::hash_combine(h, p.x);
    boost::hash_combine(h, p.y);
    return h;
}

inline bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=(const Point& p1, const Point& p2)
{
    return p1.x != p2.x || p1.y != p2.y;
}

// -*- C++ -*-

#ifndef MAP_H_
#define MAP_H_

#include <boost/utility.hpp>
#include <cassert>
#include <string>
#include <vector>

struct Point;

class MineMap : boost::noncopyable
{
public:
    explicit MineMap(const std::vector<std::string>& rows);

    char& cell(size_t x, size_t y)
    {
        assert(y < rows_.size() && x < rows_[0].length());
        return rows_[y][x];
    }

    char cell(size_t x, size_t y) const
    {
        assert(y < rows_.size() && x < rows_[0].length());
        return rows_[y][x];
    }

    size_t w() const { return rows_[0].length() - 2; }
    size_t h() const { return rows_.size() - 2; }

    Point FindCell(char c) const;

    MineMap* Clone() const { return new MineMap(*this); }

private:
    MineMap(const MineMap& base) : rows_(base.rows_) {}
    std::vector<std::string> rows_;
};

bool CanMove(const MineMap& map, int x, int y, int dx, int dy);
int GetRockMove(const MineMap& map, int x, int y);

#include "map-inl.h"

#endif // MAP_H_

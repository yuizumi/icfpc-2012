// -*- C++ -*-

#ifndef WARPS_H_
#define WARPS_H_

#include <boost/unordered_map.hpp>
#include "point.h"

class MineMap;

struct Warp { Point src, dst; };
typedef boost::unordered_map<char, Warp> Warps;

void ParseWarpSpec(const MineMap* map, const char* spec, Warps* warps);

#endif // WARPS_H_

// -*- C++ -*-

#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <ostream>

struct Point;
class State;

std::ostream& operator<<(std::ostream& out, const Point& p);
void PrintState(std::ostream& out, const State& q);

#endif // DEBUGUTIL_H_

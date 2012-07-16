// -*- C++ -*-

#include <cassert>

inline bool CanMove(const MineMap& map, int x, int y, int dx, int dy)
{
    assert(abs(dx) + abs(dy) == 1);
    x += dx, y += dy;
    char c = map.cell(x, y);
    return c == ' ' || c == '.' || c == 'O' || c == '\\' ||
           c == '!' || (c >= 'A' && c <= 'I') ||
           (c == '*' && dy == 0 && map.cell(x + dx, y) == ' ') ||
           (c == '@' && dy == 0 && map.cell(x + dx, y) == ' ');
}

inline int GetRockMove(const MineMap& map, int x, int y)
{
    switch (map.cell(x, y - 1)) {
    case ' ':
        return x;
    case '*':
    case '@':
        if (map.cell(x + 1, y) == ' ' && map.cell(x + 1, y - 1) == ' ') { return x + 1; }
        if (map.cell(x - 1, y) == ' ' && map.cell(x - 1, y - 1) == ' ') { return x - 1; }
        return 0;
    case '\\':
        if (map.cell(x + 1, y) == ' ' && map.cell(x + 1, y - 1) == ' ') { return x + 1; }
        return 0;
    default:
        return 0;
    }
}

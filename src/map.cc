#include "map.h"
#include <algorithm>
#include "point.h"

using namespace std;

MineMap::MineMap(const vector<string>& rows)
{
    size_t h = rows.size();
    size_t w = 0;
    for (size_t j = 0; j < h; j++) {
        w = max(w, rows[j].length());
    }

    rows_.assign(h + 2, string(w + 2, '~'));
    for (size_t y = 1; y <= h; y++) {
        size_t j = h - y;
        string::iterator copied =
            copy(rows[j].begin(), rows[j].end(), rows_[y].begin() + 1);
        fill(copied, rows_[y].end() - 1, ' ');
    }
}

Point MineMap::FindCell(char c) const
{
    for (size_t y = 1; y <= h(); y++) {
        size_t x = rows_[y].find(c);
        if (x != string::npos) { return Point(x, y); }
    }
    return Point(0, 0); // invalid
}

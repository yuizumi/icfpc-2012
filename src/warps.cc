#include "warps.h"
#include "map.h"

void ParseWarpSpec(const MineMap* map, const char* spec, Warps* warps)
{
    const char* p = spec;

    while (*p != '\0') {
        char key = *p;
        assert(*p >= 'A' && *p <= 'I');
        (*warps)[key].src = map->FindCell(*p++);
        assert(*p >= '1' && *p <= '9');
        (*warps)[key].dst = map->FindCell(*p++);
    }
}

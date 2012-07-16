#include "debugutil.h"
#include <string>
#include "state.h"

using namespace std;

namespace {

void PrintCommand(ostream& out, const string& command)
{
    out << "\033[36m";
    if (command.length() > 40) {
        out << "..." << command.substr(command.length() - 37);
    } else {
        out << command;
    }
    out << "\033[m" << endl;
}

void PrintRobot(ostream& out, const Robot& robot)
{
    if (robot.erosion() > robot.max_erosion() + 1) {
        out << "E:[---]";
    } else {
        out << "E:[" << robot.erosion() << "/" << robot.max_erosion() << "]";
    }
    out << " R:" << robot.razors();
}

void PrintWarps(ostream& out, const Warps& warps, const MineMap& map)
{
    if (warps.empty()) { return; }
    out << " {";
    for (char c = 'A'; c <= 'I'; c++) {
        Warps::const_iterator it = warps.find(c);
        if (it != warps.end()) {
            const Point& dst = it->second.dst;
            out << " " << c << "->" << map.cell(dst.x, dst.y);
        }
    }
    out << " }";
}

void PrintMap(ostream& out, const MineMap& map, uint water)
{
    for (int y = map.h(); y >= 1; y--) {
        out << (y <= water ? "\033[44m" : "");
        for (int x = 1; x <= map.w(); x++) { out << map.cell(x, y); }
        out << "\033[m" << endl;
    }
}

}  // namespace

ostream& operator<<(ostream& out, const Point& p)
{
    return out << "Point(" << p.x << ", " << p.y << ")";
}

void PrintState(ostream& out, const State& q)
{
    PrintCommand(out, q.commands());
    out << "\033[35m";
    PrintRobot(out, *q.robot());
    PrintWarps(out, *q.warps(), *q.map());
    out << "\033[m" << endl;
    PrintMap(out, *q.map(), q.water());
}

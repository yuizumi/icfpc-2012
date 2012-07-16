#include "state.h"
#include <cassert>
#include "metadata.h"

using namespace std;

State::State(MineMap* map, const Metadata& md)
    : map_(map), robot_(new Robot(map->FindCell('R'), md.waterproof, md.razors)),
      lift_(map->FindCell('L')), num_lams_(0),
      water_(md.water), flood_(md.flood), growth_(md.growth)
{
    assert(robot_->p() != Point(0, 0));
    assert(lift_ != Point(0, 0));

    for (int y = 1; y <= map->h(); y++)
    for (int x = 1; x <= map->w(); x++) {
        if (map->cell(x, y) == '\\' || map->cell(x, y) == '@') { ++num_lams_; }
    }

    ParseWarpSpec(map, md.trampolines.c_str(), &warps_);
}

bool State::Command(char cmd)
{
    assert(!IsGameOver());
    bool success;

    switch (cmd) {
    case 'L': success = MoveRobot(-1, 0); break;
    case 'R': success = MoveRobot(+1, 0); break;
    case 'U': success = MoveRobot(0, +1); break;
    case 'D': success = MoveRobot(0, -1); break;
    case 'S': success = UseRazor(); break;
    case 'W': success = true; break;
    default: assert(false);
    }
    commands_ += cmd;

    UpdateMap();
    UpdateWater();
    return success;
}

bool State::MoveRobot(int dx, int dy)
{
    int x0 = robot_->x(), y0 = robot_->y();

    if (!CanMoveRobot(dx, dy)) { return false; }

    int x1 = x0 + dx, y1 = y0 + dy;
    char c = map_->cell(x1, y1);

    if (c == '\\') {
        if (--num_lams_ == 0) { map_->cell(lift_.x, lift_.y) = 'O'; }
    }
    if (c == '*' || c == '@') {
        map_->cell(x1 + dx, y1 + dy) = c;
    }
    if (c == '!') {
        robot_->AcquireRazor();
    }
    if (c >= 'A' && c <= 'I') {
        x1 = warps_[c].dst.x; y1 = warps_[c].dst.y; RemoveWarps(c);
    }

    map_->cell(x0, y0) = ' ';
    map_->cell(x1, y1) = 'R';
    robot_->MoveTo(x1, y1);

    return true;
}

bool State::UseRazor()
{
    if (robot_->razors() == 0) { return false; }

    for (int dy = -1; dy <= 1; dy++)
    for (int dx = -1; dx <= 1; dx++) {
        int x = robot_->x() + dx, y = robot_->y() + dy;
        if (map_->cell(x, y) == 'W') { map_->cell(x, y) = ' '; }
    }

    robot_->ConsumeRazor();
    return true;
}

void State::UpdateMap()
{
    static vector< pair<Point, char> > rewrites;

    bool to_grow_beards = (num_steps() % growth_ == 0);

    for (int y = 1; y <= map_->h(); y++)
    for (int x = 1; x <= map_->w(); x++) {
        if (map_->cell(x, y) == '*') {
            int x_new = GetRockMove(*map_, x, y);
            if (x_new > 0) {
                rewrites.push_back(make_pair(Point(x_new, y - 1), '*'));
                rewrites.push_back(make_pair(Point(x, y), ' '));
            }
        }
        if (map_->cell(x, y) == '@') {
            int x_new = GetRockMove(*map_, x, y);
            if (x_new > 0) {
                char c_new = map_->cell(x_new, y - 2) != ' ' ? '\\' : '@';
                rewrites.push_back(make_pair(Point(x_new, y - 1), c_new));
                rewrites.push_back(make_pair(Point(x, y), ' '));
            }
        }
        if (map_->cell(x, y) == 'W' && to_grow_beards) {
            for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++) {
                int x1 = x + dx, y1 = y + dy;
                if (map_->cell(x1, y1) == ' ') {
                    rewrites.push_back(make_pair(Point(x1, y1), 'W'));
                }
            }
        }
    }

    for (size_t i = 0; i < rewrites.size(); i++) {
        const Point& p = rewrites[i].first;
        map_->cell(p.x, p.y) = rewrites[i].second;
        if (rewrites[i].second == '*' && map_->cell(p.x, p.y - 1) == 'R') {
            robot_->GetDestroyed();
        }
    }

    rewrites.clear();  // keeps allocated memory
}

void State::UpdateWater()
{
    if (robot_->IsAlive()) {
        (robot_->y() <= water_) ? robot_->GetEroded() : robot_->ResetErosion();
    }
    if (flood_ > 0) {
        if (num_steps() % flood_ == 0) { ++water_; }
    }
}

void State::RemoveWarps(char c)
{
    const Point dst = warps_[c].dst;

    Warps::iterator it = warps_.begin();
    while (it != warps_.end()) {
        const Warp& warp = it->second;
        if (warp.dst == dst) {
            map_->cell(warp.src.x, warp.src.y) = ' ';
            it = warps_.erase(it);
        } else {
            ++it;
        }
    }
}

#include <algorithm>
#include <iostream>
#include <queue>
#include "debugutil.h"
#include "solve.h"
#include "solveutil.h"
#include "state.h"

#ifdef MOREDEBUG
#define PrintState4Debug PrintState
#define PrintSteps4Debug PrintSteps
#else
#define PrintState4Debug(out, q)
#define PrintSteps4Debug(out)
#endif

using namespace std;

namespace {
// Constants -----------------------------------------------------------

const size_t kStateStoreSize = 32;

const short kEmpty = -1;
const short kRobot = -2;
const short kDown = 0;

const int kDx[] = { 0, -1, +1, 0 };
const int kDy[] = { -1, 0, 0, +1 };
const char kCmds[] = "DLRU";

// Functions -----------------------------------------------------------

template <class Collection, typename Value>
inline bool Contains(const Collection& c, const Value& value)
{
    return c.find(value) != c.end();
}

inline bool IsRock(char c)
{
    return c == '*' || c == '@';
}

inline bool IsNearToBeard(const MineMap& map, int x, int y)
{
    for (int dy = -1; dy <= 1; dy++)
    for (int dx = -1; dx <= 1; dx++) {
        if (map.cell(x + dx, y + dy) == 'W') { return true; }
    }
    return false;
}

inline bool IsSquashed(const MineMap& map, int x, int y, short k)
{
    if (k != kDown) { return false; }
    return IsRock(map.cell(x, y + 1))
        || (IsRock(map.cell(x + 1, y + 1)) && IsRock(map.cell(x + 1, y)))
        || (IsRock(map.cell(x - 1, y + 1)) && IsRock(map.cell(x - 1, y)))
        || (IsRock(map.cell(x - 1, y + 1)) && map.cell(x - 1, y) == '\\');
}

inline bool ToGetLambda(const MineMap& map, int x, int y, short k)
{
    return map.cell(x, y) == 'O' || map.cell(x, y) == '\\' ||
           (kDy[k] == 0 && map.cell(x - kDx[k], y + 1) == '@') ||
           // (x, y) will be occupied with the robot, so this should be fine.
           (map.cell(x, y) == '@' && GetRockMove(map, x + kDx[k], y));
}

inline bool ToApproachRock(const MineMap& map, int x, int y, short k)
{
    return map.cell(x, y) != ' ' && (
        IsRock(map.cell(x - 1, y)) || IsRock(map.cell(x + 1, y)) ||
        IsSquashed(map, x, y, kDown));
}

inline bool ToGetRazor(const MineMap& map, int x, int y, short k)
{
    return map.cell(x, y) == '!';
}

inline bool ToShaveBeard(const MineMap& map, int x, int y, short k)
{
    int wx, wy, num = 0;

    for (int dy = -1; dy <= 1; dy++)
    for (int dx = -1; dx <= 1; dx++) {
        if (map.cell(x + dx, y + dy) == 'W') {
            wx = x + dx;
            wy = y + dy;
            ++num;
        }
    }

    if (num == 0) { return false; }
    if (num >= 2) { return true; }

    for (int dy = -1; dy <= 1; dy++)
    for (int dx = -1; dx <= 1; dx++) {
        if (!(dx == 0 || dy == 0) && map.cell(wx + dx, wy + dy) == 'W') {
            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------
} // namespace


Solver::Solver(State* state)
    : num_lams_(state->num_lams()), max_score_(0), store_(kStateStoreSize),
      steps_(state->map()->h() + 2, vector<short>(state->map()->w() + 2, kEmpty))
{
    store_.Push(new Snapshot(state));
}

void Solver::Solve()
{
#ifdef EARLY_EXIT
    while (store_.Top() && !state()->IsSuccess()) {
#else
    while (store_.Top()) {
#endif
        PrintState4Debug(cout, *state());

        bool beards = state()->map()->FindCell('W') != Point(0, 0);
        Point target;
        bool found = ((beards && state()->robot()->razors() > 0 &&
                       FindSubgoal(&target, ToShaveBeard)) ||
                      (beards && FindSubgoal(&target, ToGetRazor)) ||
                      FindSubgoal(&target, ToGetLambda) ||
                      (store_.Top()->blocked.empty() &&
                       FindSubgoal(&target, ToApproachRock)));
        if (found) {
            ApplySteps(target);
            UpdateOutput();
        } else {
            PrintSteps4Debug(cout);
            DiscardState();
        }
    }

    if (store_.Top()) { PrintState4Debug(cout, *state()); }
}

template <typename IsSubgoalFunc>
bool Solver::FindSubgoal(Point *dst, IsSubgoalFunc is_subgoal)
{
    ClearSteps();

    const Point& start = state()->robot()->p();
    steps(start.x, start.y) = kRobot;

    queue<Point> cue;
    cue.push(start);

    const MineMap* map = state()->map();

    while (!cue.empty()) {
        Point p = cue.front();
        cue.pop();

        for (short k = 0; k < 4; k++) {
            int x = p.x + kDx[k];
            int y = p.y + kDy[k];

            char c = map->cell(x, y);
            bool can_likely_move =
                steps(x, y) == kEmpty &&
                !Contains(store_.Top()->blocked, Point(x, y)) &&
                CanMove(*map, p.x, p.y, kDx[k], kDy[k]) &&
                !IsSquashed(*map, p.x, p.y, k);
            if (!can_likely_move) { continue; }

            steps(x, y) = k;

            if (is_subgoal(*map, x, y, k)) {
                dst->x = x;
                dst->y = y;
                return true;
            }

            if (c >= 'A' && c <= 'I') {
                const Point& dst = (*state()->warps()).find(c)->second.dst;
                steps(dst.x, dst.y) = c;
                cue.push(dst);
            } else {
                cue.push(Point(x, y));
            }
        }
    }

    return false;
}

bool Solver::ApplySteps(const Point& dst)
{
    int x = dst.x, y = dst.y;
    string cmds = "";

    if (IsNearToBeard(*state()->map(), x, y)) {
        cmds += 'S';
    }

    while (steps(x, y) != kRobot) {
        if (steps(x, y) >= 'A' && steps(x, y) <= 'I') {
            char c = static_cast<char>(steps(x, y));
            const Point& src = (*state()->warps()).find(c)->second.src;
            x = src.x, y = src.y;
        }
        short k = steps(x, y);
        cmds += kCmds[k], x -= kDx[k], y -= kDy[k];
    }

    store_.Push(new Snapshot(state()->Clone()));

    bool error = false;
    for (string::const_reverse_iterator it = cmds.rbegin();
         it != cmds.rend(); ++it) {
        if (state()->IsGameOver() || !state()->Command(*it)) {
            error = true;
            break;
        }
    }

    if (error || state()->IsFailure()) {
        PrintState4Debug(cout, *state());
        DiscardState();
        return false;
    }

    return true;
}

void Solver::ClearSteps()
{
    for (size_t i = 0; i < steps_.size(); i++) {
        fill(steps_[i].begin(), steps_[i].end(), kEmpty);
    }
}

void Solver::UpdateOutput()
{
    int base = state()->IsSuccess() ? 75 :50;
    int score =
        (num_lams_ - state()->num_lams()) * base - state()->num_steps();

    if (score > max_score_) {
        SaveOutput(state()->commands());
        max_score_ = score;
    }
}

void Solver::DiscardState()
{
    Point p = state()->robot()->p();
    store_.Pop();
    if (store_.Top()) {
        store_.Top()->blocked.insert(p);
    }
}

void Solver::PrintSteps(ostream& out) const
{
    size_t w = steps_[0].size() - 2;
    size_t h = steps_.size() - 2;

    for (size_t y = h; y >= 1; y--) {
        for (size_t x = 1; x <= w; x++) {
            if (Contains(store_.Top()->blocked, Point(x, y))) {
                cout << "*";
                continue;
            }
            switch (steps_[y][x]) {
            case 0: cout << "v"; break;
            case 1: cout << "<"; break;
            case 2: cout << ">"; break;
            case 3: cout << "^"; break;
            case kEmpty: cout << " "; break;
            case kRobot: cout << "0"; break;
            default: cout << static_cast<char>(steps_[y][x]); break;
            }
        }
        cout << "|" << endl;
    }
}

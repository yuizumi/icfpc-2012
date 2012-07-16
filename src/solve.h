// -*- C++ -*-

#ifndef SOLVE_H_
#define SOLVE_H_

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility.hpp>
#include <ostream>
#include <vector>
#include "state.h"
#include "store.h"

struct Snapshot
{
    explicit Snapshot(State* state) : state(state), blocked() {}

    boost::scoped_ptr<State> state;
    boost::unordered_set<Point> blocked;
};

class Solver : boost::noncopyable
{
public:
    explicit Solver(State* state);

    void Solve();
    int max_score() const { return max_score_; }

private:
    State* state() { return store_.Top()->state.get(); }

    short steps(int x, int y) const { return steps_[y][x]; }
    short& steps(int x, int y) { return steps_[y][x]; }

    template <typename IsSubgoalFunc>
    bool FindSubgoal(Point *dst, IsSubgoalFunc is_subgoal);

    bool ApplySteps(const Point& dst);
    void ClearSteps();

    void UpdateOutput();
    void DiscardState();

    void PrintSteps(std::ostream& out) const;

    const int num_lams_;
    int max_score_;
    Store<Snapshot> store_;
    std::vector< std::vector<short> > steps_;
};

#endif // SOLVE_H_

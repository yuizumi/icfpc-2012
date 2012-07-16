// -*- C++ -*-

#ifndef STATE_H_
#define STATE_H_

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include "basic.h"
#include "map.h"
#include "point.h"
#include "robot.h"
#include "warps.h"

struct Metadata;

class State : boost::noncopyable
{
public:
    // map is owned and altered by this object.
    State(MineMap* map, const Metadata& md);

    const MineMap* map() const { return map_.get(); }
    const Robot* robot() const { return robot_.get(); }
    uint num_lams() const { return num_lams_; }
    uint water() const { return water_; }
    uint flood() const { return flood_; }
    uint growth() const { return growth_; }
    const Warps* warps() const { return &warps_; }
    std::string commands() const { return commands_; }

    size_t num_steps() const { return commands_.length(); }

    bool IsSuccess() const;
    bool IsFailure() const;
    bool IsGameOver() const;

    bool Command(char cmd);

    State* Clone() const { return new State(*this); }

private:
    bool CanMoveRobot(int dx, int dy) const;
    bool MoveRobot(int dx, int dy);
    bool UseRazor();
    void UpdateMap();
    void UpdateWater();
    void RemoveWarps(char c);

    State(const State& base)  // for Clone()
        : map_(base.map_->Clone()), robot_(base.robot_->Clone()),
          lift_(base.lift_), num_lams_(base.num_lams_),
          water_(base.water_), flood_(base.flood_), growth_(base.growth_),
          warps_(base.warps_), commands_(base.commands_) {}

    boost::scoped_ptr<MineMap> map_;
    boost::scoped_ptr<Robot> robot_;
    const Point lift_;
    uint num_lams_;
    uint water_;
    const uint flood_;
    const uint growth_;
    Warps warps_;
    std::string commands_;
};

#include "state-inl.h"

#endif // STATE_H_

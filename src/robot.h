// -*- C++ -*-

#ifndef ROBOT_H_
#define ROBOT_H_

#include <boost/utility.hpp>
#include "basic.h"
#include "point.h"

class Robot : boost::noncopyable
{
public:
    Robot(const Point& p, uint max_erosion, uint razors)
        : p_(p), erosion_(0), max_erosion_(max_erosion), razors_(razors) {}

    const Point& p() const { return p_; }
    int x() const { return p_.x; }
    int y() const { return p_.y; }
    uint erosion() const { return erosion_; }
    uint max_erosion() const { return max_erosion_; }
    uint razors() const { return razors_; }

    bool IsAlive() const;

    void MoveTo(int x, int y);
    void ResetErosion();
    void GetEroded();
    void GetDestroyed();  // erosion_ to UINT_MAX

    void AcquireRazor();
    void ConsumeRazor();

    Robot* Clone() const { return new Robot(*this); }

private:
    Robot(const Robot& base)  // for Clone()
        : p_(base.p_), erosion_(base.erosion_),
          max_erosion_(base.max_erosion_), razors_(base.razors_) {}

    Point p_;
    uint erosion_;
    const uint max_erosion_;
    uint razors_;
};

#include "robot-inl.h"

#endif // ROBOT_H_

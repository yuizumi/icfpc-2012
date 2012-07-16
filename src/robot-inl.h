// -*- C++ -*-

#include <cassert>
#include <climits>

inline bool Robot::IsAlive() const
{
    return erosion_ <= max_erosion_;
}

inline void Robot::MoveTo(int x, int y)
{
    assert(IsAlive());
    p_.x = x;
    p_.y = y;
}

inline void Robot::ResetErosion()
{
    assert(IsAlive());
    erosion_ = 0;
}

inline void Robot::GetEroded()
{
    assert(IsAlive());
    ++erosion_;
}

inline void Robot::GetDestroyed()
{
    assert(IsAlive() || erosion_ == UINT_MAX);
    erosion_ = UINT_MAX;
}

inline void Robot::AcquireRazor()
{
    assert(IsAlive());
    ++razors_;
}

inline void Robot::ConsumeRazor()
{
    assert(IsAlive() && razors_ > 0);
    --razors_;
}

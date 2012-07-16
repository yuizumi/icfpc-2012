// -*- C++ -*-

#include <cassert>

inline bool State::IsSuccess() const
{
    return robot_->p() == lift_;
}

inline bool State::IsFailure() const
{
    return !robot_->IsAlive() && !IsSuccess();
}

inline bool State::IsGameOver() const
{
    return IsSuccess() || IsFailure() || num_steps() >= map_->w() * map_->h();
}

inline bool State::CanMoveRobot(int dx, int dy) const
{
    assert(!IsGameOver());
    return CanMove(*map_, robot_->x(), robot_->y(), dx, dy);
}

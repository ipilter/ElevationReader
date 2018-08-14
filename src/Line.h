#pragma once

#include "Geo.h"

class Line
{
public:
  Line(const Geo& leftTop, const Geo& rightBottom)
    : mLeftTop(leftTop)
    , mRightBottom(rightBottom)
  { }
  Geo intersect(const Line& rhs) const
  {
    const double denominator((mLeftTop.lon - mRightBottom.lon) * (rhs.mLeftTop.lat - rhs.mRightBottom.lat) -
                             (mLeftTop.lat - mRightBottom.lat) * (rhs.mLeftTop.lon - rhs.mRightBottom.lon));
    if (!Equal(denominator, 0.0))
    {
      const double f1(mLeftTop.lon * mRightBottom.lat - mLeftTop.lat * mRightBottom.lon);
      const double f2(rhs.mLeftTop.lon * rhs.mRightBottom.lat - rhs.mLeftTop.lat * rhs.mRightBottom.lon);
      return Geo((f1 * (rhs.mLeftTop.lon - rhs.mRightBottom.lon) - (mLeftTop.lon - mRightBottom.lon) * f2) / denominator,
                 (f1 * (rhs.mLeftTop.lat - rhs.mRightBottom.lat) - (mLeftTop.lat - mRightBottom.lat) * f2) / denominator);
    }
    return Geo::invalid();
  }
  const Geo& leftTop() const
  {
    return mLeftTop;
  }
  const Geo& rightBottom() const
  {
    return mRightBottom;
  }
private:
  const Geo& mLeftTop;
  const Geo& mRightBottom;
};

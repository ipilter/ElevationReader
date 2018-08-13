#pragma once

#include "Geo.h"

class Line
{
public:
  Line(const Geo& leftTop, const Geo& rightBottom)
    : mLeftTop(leftTop)
    , mRightBottom(rightBottom)
  { }
  Geo intersect(const Line& line) const
  {
    const double denominator((mLeftTop.lon - mRightBottom.lon) * (line.mLeftTop.lat - line.mRightBottom.lat) -
                             (mLeftTop.lat - mRightBottom.lat) * (line.mLeftTop.lon - line.mRightBottom.lon));
    if (!Equal(denominator, 0.0))
    {
      const double f1(mLeftTop.lon * mRightBottom.lat - mLeftTop.lat * mRightBottom.lon);
      const double f2(line.mLeftTop.lon * line.mRightBottom.lat - line.mLeftTop.lat * line.mRightBottom.lon);
      return Geo((f1 * (line.mLeftTop.lon - line.mRightBottom.lon) - (mLeftTop.lon - mRightBottom.lon) * f2) / denominator,
                 (f1 * (line.mLeftTop.lat - line.mRightBottom.lat) - (mLeftTop.lat - mRightBottom.lat) * f2) / denominator);
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

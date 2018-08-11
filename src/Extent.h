#pragma once

#include <stdexcept>
#include <map>
#include <vector>

#include "Geo.h"

class Extent
{
public:
  Extent(const double& left
         , const double& top
         , const double right
         , const double bottom)
    : mTopLeft(left, top)
    , mBottomRight(right, bottom)
  { 
    validate();
  }
  Extent(const Geo& tl
         , const Geo& br)
    : mTopLeft(tl)
    , mBottomRight(br)
  { 
    validate();
  }
public:
  const Geo& topLeft() const
  {
    return mTopLeft;
  }
  const Geo& bottomRight() const
  {
    return mBottomRight;
  }
  const Geo topRight() const
  {
    return Geo(mBottomRight.lon, mTopLeft.lat);
  }
  const Geo bottomLeft() const
  {
    return Geo(mTopLeft.lon, mBottomRight.lat);
  }
  double left() const
  {
    return mTopLeft.lon;
  }
  double top() const
  {
    return mTopLeft.lat;
  }
  double right() const
  {
    return mBottomRight.lon;
  }
  double bottom() const
  {
    return mBottomRight.lat;
  }
  Extent intersect(const Extent& rhs) const
  {
    return Extent(0, 0, 0, 0);
  }
private:
  void validate() const
  {
    mTopLeft.validate();
    mBottomRight.validate();
    if(mTopLeft.lon > mBottomRight.lon
       || mTopLeft.lat < mBottomRight.lat)
    {
      throw std::runtime_error("invalid extent coordinates");
    }
  }
private:
  Geo mTopLeft;
  Geo mBottomRight;
};

inline std::ostream& operator << (std::ostream& stream, const Extent& extent)
{
  return stream << "TopLeft = " << extent.topLeft() << " BottomRight = " << extent.bottomRight();
}

inline bool operator == (const Extent& a, const Extent& b)
{
  return a.topLeft() == b.topLeft() && a.bottomRight() == b.bottomRight();
}

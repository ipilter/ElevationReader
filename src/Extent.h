#pragma once

#include <stdexcept>
#include <map>
#include <vector>

#include "Geo.h"
#include "Line.h"

class Extent
{
public:
  Extent(const double& left
         , const double& top
         , const double right
         , const double bottom)
    : mLeftTop(left, top)
    , mRightBottom(right, bottom)
  { 
    validate();
  }
  Extent(const Geo& lt
         , const Geo& rb)
    : mLeftTop(lt)
    , mRightBottom(rb)
  { 
    validate();
  }
public:
  const Geo& leftTop() const
  {
    return mLeftTop;
  }
  const Geo& bottomRight() const
  {
    return mRightBottom;
  }
  const Geo topRight() const
  {
    return Geo(mRightBottom.lon, mLeftTop.lat);
  }
  const Geo bottomLeft() const
  {
    return Geo(mLeftTop.lon, mRightBottom.lat);
  }
  double left() const
  {
    return mLeftTop.lon;
  }
  double top() const
  {
    return mLeftTop.lat;
  }
  double right() const
  {
    return mRightBottom.lon;
  }
  double bottom() const
  {
    return mRightBottom.lat;
  }
  Extent intersect(const Extent& rhs) const
  {
    return Extent(0, 0, 0, 0);
  }
private:
  void validate() const
  {
    mLeftTop.validate();
    mRightBottom.validate();
    if(mLeftTop.lon > mRightBottom.lon
       || mLeftTop.lat < mRightBottom.lat)
    {
      throw std::runtime_error("invalid extent coordinates");
    }
  }
private:
  Geo mLeftTop;
  Geo mRightBottom;
};

inline std::ostream& operator << (std::ostream& stream, const Extent& extent)
{
  return stream << "TopLeft = " << extent.leftTop() << " BottomRight = " << extent.bottomRight();
}

inline bool operator == (const Extent& a, const Extent& b)
{
  return a.leftTop() == b.leftTop() && a.bottomRight() == b.bottomRight();
}

#pragma once

#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>

#include "Geo.h"
#include "Line.h"

class Extent
{
public:
  Extent(const double& left
         , const double& top
         , const double& right
         , const double& bottom)
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
  const Geo& rightBottom() const
  {
    return mRightBottom;
  }
  const Geo rightTop() const
  {
    return Geo(mRightBottom.lon, mLeftTop.lat);
  }
  const Geo leftBottom() const
  {
    return Geo(mLeftTop.lon, mRightBottom.lat);
  }
  const double& left() const
  {
    return mLeftTop.lon;
  }
  const double& top() const
  {
    return mLeftTop.lat;
  }
  const double& right() const
  {
    return mRightBottom.lon;
  }
  const double& bottom() const
  {
    return mRightBottom.lat;
  }
  bool overlap(const Extent& rhs) const
  {
    return !(mLeftTop.lon >= rhs.mRightBottom.lon ||
             rhs.mLeftTop.lon >= mRightBottom.lon ||
             mLeftTop.lat <= rhs.mRightBottom.lat ||
             rhs.mLeftTop.lat <= mRightBottom.lat);
  }
  Extent intersect(const Extent& rhs) const
  {
    if (!overlap(rhs))
    {
      return invalid();
    }
    return Extent(Geo(std::max(left(), rhs.left()),
                      std::min(top(), rhs.top())),
                  Geo(std::min(right(), rhs.right()),
                      std::max(bottom(), rhs.bottom())));
  }
  static const Extent& invalid()
  {
    static const Extent invalidValue(0, 0, 0, 0);
    return invalidValue;
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
  return stream << "TopLeft = " << extent.leftTop() << " BottomRight = " << extent.rightBottom();
}

inline bool operator == (const Extent& a, const Extent& b)
{
  return a.leftTop() == b.leftTop() && a.rightBottom() == b.rightBottom();
}

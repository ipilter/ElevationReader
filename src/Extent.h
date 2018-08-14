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
  bool overlap(const Extent& rhs) const
  {
    return !(mLeftTop.lon >= rhs.mRightBottom.lon || rhs.mLeftTop.lon >= mRightBottom.lon ||
             mLeftTop.lat <= rhs.mRightBottom.lat || rhs.mLeftTop.lat <= mRightBottom.lat);
  }
  Extent intersect(const Extent& rhs) const
  {
    const Extent& lhs(*this);

    const Line lhs0(lhs.leftTop(), lhs.rightTop());
    const Line lhs1(lhs.leftTop(), lhs.leftBottom());
    const Line lhs2(lhs.rightBottom(), lhs.leftBottom());
    const Line lhs3(lhs.rightBottom(), lhs.rightTop());

    const Line rhs0(rhs.leftTop(), rhs.rightTop());
    const Line rhs1(rhs.leftTop(), rhs.leftBottom());
    const Line rhs2(rhs.rightBottom(), rhs.leftBottom());
    const Line rhs3(rhs.rightBottom(), rhs.rightTop());

    return invalid();
  }
  static Extent invalid()
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

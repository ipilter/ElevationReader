#pragma once

#include <stdexcept>

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

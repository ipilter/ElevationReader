#pragma once

#include <vector>
#include <memory>

#include "Types.h"

class Elevation
{
public:
  typedef std::shared_ptr<Elevation> Ptr;
public:
  Elevation()
    : mData()
  {}
public:
  short getElevation(int x, int y) const
  {
    return mData[x + mSize.x * y];
  }
  void setElevation(int x, int y, const short value)
  {
    mData[x + mSize.x * y] = value;
  }
  const Vec2i& getSize() const
  {
    return mSize;
  }
  void resize(const int w, const int h)
  {
    mSize = Vec2i(w, h);
    if (!mData.empty())
    {
      mData.clear();
    }
    mData.resize(mSize.x * mSize.y);
  }
  void clear()
  {
    mSize = Vec2i(0);
    mData.clear();
  }
  static short getNoDataValue()
  {
    return static_cast<short>(9999);
  }
private:
  Vec2i mSize;
  std::vector<short> mData;
};

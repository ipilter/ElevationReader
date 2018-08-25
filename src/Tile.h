#pragma once

#include <memory>
#include <vector>
#include "GeoCell.h"

class Tile
{
public:
  typedef std::shared_ptr<Tile> Ptr;
  typedef std::vector<Ptr> Vector;
public:
  Tile(const GeoCell::Ptr& geoCell, const Extent& extent)
    : mGeoCell(geoCell)
    , mExtent(extent)
  { }
  ~Tile()
  { }
public:
  const GeoCell::Ptr geoCell() const
  {
    return mGeoCell;
  }
  const Extent& extent() const
  {
    return mExtent;
  }
private:
  GeoCell::Ptr mGeoCell;
  Extent mExtent;
};

#pragma once

#include "Types.h"
#include "Extent.h"

class GeoReference
{
public:
  GeoReference(double* geo
               , int width
               , int height)
    : mImageToGeoTransform(geo[1], geo[2]
                           , geo[4], geo[5]
                           , geo[0], geo[3])
    , mGeoToImageTransform(inverse(mImageToGeoTransform))
    , mImageSize(width, height)
    , mExtent(imgToGeo(Vec2i(0, 0)), imgToGeo(mImageSize))
  { }
  Vec2i geoToImg(const Geo& geo) const
  {
    return Vec2i(mGeoToImageTransform * Vec3(geo.lon, geo.lat, 1.0));
  }
  Geo imgToGeo(const Vec2i& img) const
  {
    return Geo(mImageToGeoTransform * Vec3(img, 1.0));
  }
  const Vec2i& imageSize() const
  {
    return mImageSize;
  }
  const Extent& extent() const
  {
    return mExtent;
  }
private:
  const Mat32 mImageToGeoTransform;
  const Mat32 mGeoToImageTransform;
  const Vec2i mImageSize;
  const Extent mExtent;
};

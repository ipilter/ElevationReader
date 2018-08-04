#pragma once

#include "Types.h"
#include "Extent.h"

class GeoReference
{
public:
  GeoReference(double* geo
               , double* inv
               , int width
               , int height)
    : mGeoToImageTransform(inv[1], inv[2], inv[4]
                           , inv[5], inv[0], inv[3])
    , mImageToGeoTransform(geo[1], geo[2], geo[4]
                           , geo[5], geo[0], geo[3])
    , mImageSize(width, height)
    , mExtent(imgToGeo(Vec2i(0, 0))
              , imgToGeo(mImageSize))
  { }
  Vec2i geoToImg(const Geo& geo) const
  {
    return Vec2i(mGeoToImageTransform * Vec3(geo.lon, geo.lat, 1.0));
  }
  Geo imgToGeo(const Vec2i& img) const
  {
    const Vec2 vi(mImageToGeoTransform * Vec3(img, 1.0));
    return Geo(vi.x, vi.y);
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
  Mat3x2 mGeoToImageTransform;
  Mat3x2 mImageToGeoTransform;
  Vec2i mImageSize;
  Extent mExtent;
};

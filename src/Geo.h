#pragma once

#include "Floating.h"

struct Geo
{
  Geo()
    : lon(0.0)
    , lat(0.0)
  { }
  Geo(const double x
      , const double y)
    : lon(x)
    , lat(y)
  { }
  void validate() const
  {
    static const double maxLon(180.0);
    static const double maxLat(90.0);
    if (lon < -maxLon || lon > maxLon || lat < -maxLat || lat > maxLat)
    {
      throw std::runtime_error("invalid Geo coordinate");
    }
  }

  double lon;
  double lat;
};

inline bool operator == (const Geo& a, const Geo& b)
{
  return Equal(a.lon, b.lon) && Equal(a.lat, b.lat);
}

inline std::ostream& operator << (std::ostream& stream, const Geo& geo)
{
  return stream << std::fixed << "[" << geo.lon << ", " << geo.lat << "]";
}

#pragma once

#include "Types.h"
#include "Numbers.h"

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
  explicit Geo(const Vec2& v)
    : lon(v.x)
    , lat(v.y)
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
  Geo& operator *= (const double& scale)
  {
    lon *= scale;
    lat *= scale;
    return *this;
  }
  static const Geo& invalid()
  {
    static const Geo invalidValue(1000, 1000);
    return invalidValue;
  };
  double lon;
  double lat;
};

inline bool operator == (const Geo& a, const Geo& b)
{
  return Equal(a.lon, b.lon) && Equal(a.lat, b.lat);
}

inline bool operator < (const Geo& a, const Geo& b)
{
  if (a.lon < b.lon)
  {
    return true;
  }
  if (Equal(a.lon, b.lon))
  {
    return a.lat < b.lat;
  }
  return false;
}

inline std::ostream& operator << (std::ostream& stream, const Geo& geo)
{
  if (geo == Geo::invalid())
  {
    return stream << std::fixed << "[invalid]";
  }
  return stream << std::fixed << "[" << geo.lon << ", " << geo.lat << "]";
}

inline Geo operator - (const Geo& lhs, const Geo& rhs)
{
  return Geo(lhs.lon - rhs.lon, lhs.lat - rhs.lat);
}

inline Geo operator + (const Geo& lhs, const Geo& rhs)
{
  return Geo(lhs.lon + rhs.lon, lhs.lat + rhs.lat);
}

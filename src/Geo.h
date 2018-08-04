#pragma once

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

  double lon;
  double lat;
};

std::ostream& operator << (std::ostream& stream, const Geo& geo)
{
  return stream << std::fixed << "[" << geo.lat << ", " << geo.lon << "]";
}

#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#include "GeoReference.h"

#include "Types.h"

struct GeoCell
{
  typedef GeoCell Self;
  typedef std::shared_ptr<Self> Ptr;
  typedef std::vector<Ptr> Vector;

  GeoCell()
    : leftBottom(0, 0)
  {
    validate();
  }
  GeoCell(const int x, const int y)
    : leftBottom(x, y)
  {
    validate();
  }
  explicit GeoCell(const Vec2i& v)
    : leftBottom(v)
  { 
    validate();
  }
  void setGeoReference(const GeoReference::Ptr& geoReference)
  {
    mGeoReference = geoReference;
  }
  const GeoReference::Ptr& geoReference() const
  {
    return mGeoReference;
  }
  std::string asString() const
  {
    static const std::string ewChars("EW");
    static const std::string nsChars("NS");
    const size_t lonIdx(leftBottom.x >= 0.0 ? 0 : 1);
    const size_t latIdx(leftBottom.y >= 0.0 ? 0 : 1);

    // stringify as Lat-Lon format
    std::stringstream ss;
    ss << nsChars[latIdx] << std::setw(3) << std::setfill('0') << std::abs(leftBottom.y)
       << ewChars[lonIdx] << std::setw(3) << std::setfill('0') << std::abs(leftBottom.x);
    return ss.str();
  }
private:
  void validate()
  {
    static const int maxLon(179);
    static const int maxLat(89);
    if (leftBottom.x < -maxLon || leftBottom.x > maxLon || leftBottom.y < -maxLat || leftBottom.y > maxLat)
    {
      throw std::runtime_error("invalid geocell coordinate: " + asString());
    }
  }
public:
  Vec2i leftBottom;
  GeoReference::Ptr mGeoReference;
};

inline std::ostream& operator << (std::ostream& stream, const GeoCell& geocell)
{
  return stream << "[" << geocell.asString() << "]";
}

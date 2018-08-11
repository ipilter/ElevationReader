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
    : bottomLeft(0, 0)
  {
    validate();
  }
  GeoCell(const int x, const int y)
    : bottomLeft(x, y)
  {
    validate();
  }
  explicit GeoCell(const Vec2i& v)
    : bottomLeft(v)
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
    const size_t lonIdx(bottomLeft.x >= 0.0 ? 0 : 1);
    const size_t latIdx(bottomLeft.y >= 0.0 ? 0 : 1);

    // stringify as Lat-Lon format
    std::stringstream ss;
    ss << nsChars[latIdx] << std::setw(3) << std::setfill('0') << std::abs(bottomLeft.y)
       << ewChars[lonIdx] << std::setw(3) << std::setfill('0') << std::abs(bottomLeft.x);
    return ss.str();
  }
private:
  void validate()
  {
    static const int maxLon(179);
    static const int maxLat(89);
    if (bottomLeft.x < -maxLon || bottomLeft.x > maxLon || bottomLeft.y < -maxLat || bottomLeft.y > maxLat)
    {
      throw std::runtime_error("invalid geocell coordinate: " + asString());
    }
  }
public:
  Vec2i bottomLeft;
  GeoReference::Ptr mGeoReference;
};

inline std::ostream& operator << (std::ostream& stream, const GeoCell& geocell)
{
  return stream << "[" << geocell.asString() << "]";
}

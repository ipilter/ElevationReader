#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "Types.h"

struct GeoCell
{
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
      throw std::runtime_error("invalid geocell: " + asString());
    }
  }
public:
  Vec2i bottomLeft;
};

inline std::ostream& operator << (std::ostream& stream, const GeoCell& geocell)
{
  return stream << "[" << geocell.asString() << "]";
}

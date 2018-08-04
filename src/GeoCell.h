#pragma once

#include <string>
#include <sstream>
#include <iomanip>

struct GeoCell
{
  GeoCell()
    : bottomLeft(0, 0)
  { }
  GeoCell(const int x, const int y)
    : bottomLeft(x, y)
  { }
  explicit GeoCell(const Vec2i& v)
    : bottomLeft(v)
  { }
  std::string asString() const
  {
    static const std::string directionCharacters("EWNS");
    const size_t latIdx(bottomLeft.x >= 0.0 ? 0 : 1);
    const size_t lonIdx(bottomLeft.y >= 0.0 ? 2 : 3);

    std::stringstream ss;
    ss << directionCharacters[lonIdx] << std::setw(3) << std::setfill('0') << std::abs(bottomLeft.y)
       << directionCharacters[latIdx] << std::setw(3) << std::setfill('0') << std::abs(bottomLeft.x);
    return ss.str();
  }
  Vec2i bottomLeft;
};

std::ostream& operator << (std::ostream& stream, const GeoCell& geocell)
{
  return stream << "[" << geocell.asString() << "]";
}

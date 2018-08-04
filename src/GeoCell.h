#pragma once

#include <string>
#include <sstream>
#include <iomanip>

struct GeoCell
{
  GeoCell()
    : left(0)
    , bottom(0)
  { }
  GeoCell(const int x, const int y)
    : left(x)
    , bottom(y)
  { }
  std::string asString() const
  {
    static const std::string directionCharacters("NSEW");
    const size_t lonIdx(bottom >= 0.0 ? 0 : 1);
    const size_t latIdx(left >= 0.0 ? 2 : 3);

    std::stringstream ss;
    ss << directionCharacters[lonIdx] << std::setw(3) << std::setfill('0') << std::abs(bottom)
       << directionCharacters[latIdx] << std::setw(3) << std::setfill('0') << std::abs(left);
    return ss.str();
  }
  int left;
  int bottom;
};

std::ostream& operator << (std::ostream& stream, const GeoCell& geocell)
{
  return stream << "[" << geocell.asString() << "]";
}

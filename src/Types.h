#pragma once

#include <ostream>
#include <glm.hpp>

typedef glm::ivec2 Vec2i;
typedef glm::dvec2 Vec2;
typedef glm::dvec3 Vec3;
typedef glm::mat3x2 Mat3x2;

std::ostream& operator << (std::ostream& stream, const Vec2i& v)
{
  return stream << "[" << v.x << ", " << v.y << "]";
}

std::ostream& operator << (std::ostream& stream, const Vec2& v)
{
  return stream << std::fixed << "[" << v.x << ", " << v.y << "]";
}

std::ostream& operator << (std::ostream& stream, const Vec3& v)
{
  return stream << std::fixed << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

#pragma once

#include <ostream>
#include <glm.hpp>

typedef glm::ivec2 Vec2i;
typedef glm::dvec2 Vec2;
typedef glm::fvec2 Vec2f;

typedef glm::ivec3 Vec3i;
typedef glm::dvec3 Vec3;
typedef glm::fvec3 Vec3f;

typedef glm::mat3x2 Mat32;
typedef glm::mat3x3 Mat33;

inline std::ostream& operator << (std::ostream& stream, const Vec2i& v)
{
  return stream << "[" << v.x << ", " << v.y << "]";
}

inline std::ostream& operator << (std::ostream& stream, const Vec2& v)
{
  return stream << std::fixed << "[" << v.x << ", " << v.y << "]";
}

inline std::ostream& operator << (std::ostream& stream, const Vec3& v)
{
  return stream << std::fixed << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

inline Mat32 inverse(const Mat32& m)
{
  return Mat32(glm::inverse(Mat33(m))); 
}

#pragma once

#include "Types.h"

// ughly copy of vertices
// floating point vertices used to be stl compatible
class Triangle
{
public:
  Triangle(const Vec3f& v0
           , const Vec3f& v1
           , const Vec3f& v2)
  {
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    
    mNormal = glm::normalize(glm::cross((v0 - v1), (v0 - v2)));
  }
  const Vec3f& v0() const
  {
    return mVertices[0];
  }
  const Vec3f& v1() const
  {
    return  mVertices[1];
  }
  const Vec3f& v2() const
  {
    return  mVertices[2];
  }
  const Vec3f& operator[](const unsigned i) const
  {
    return mVertices[i];
  }
  Vec3f& operator[](const unsigned i)
  {
    return mVertices[i];
  }
  const Vec3f& normal() const
  {
    return mNormal;
  }
private:
  Vec3f mNormal;
  Vec3f mVertices[3];
};

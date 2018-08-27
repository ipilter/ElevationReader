#pragma once

#include <fstream>
#include <string>

#include "Types.h"

namespace io
{
  template<class T>
  void write(std::ofstream& stream, const T& t)
  {
    stream.write(reinterpret_cast<const char*>(&t), sizeof(T));
  }

  template<>
  void write(std::ofstream& stream, const Vec3& v)
  {
    stream.write(reinterpret_cast<const char*>(&v.x), sizeof(Vec3::value_type));
    stream.write(reinterpret_cast<const char*>(&v.y), sizeof(Vec3::value_type));
    stream.write(reinterpret_cast<const char*>(&v.z), sizeof(Vec3::value_type));
  }

  // cannot read back.. save size
  template<>
  void write(std::ofstream& stream, const std::string& str)
  {
    stream.write(reinterpret_cast<const char*>(str.c_str()), str.size());
  }
}

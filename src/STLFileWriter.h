#pragma once

#include <vector>
#include <fstream>

#include "Triangle.h"
#include "LowIO.h"

class STLFileWriter
{
  typedef std::vector<Triangle> Triangles;
public:
  STLFileWriter(const size_t faceCount)
  {
    mTriangles.reserve(faceCount);
  }
public:
  void addTriangle(const Triangle& triangle)
  {
    mTriangles.push_back(triangle);
  }

  void write(const std::string& path)
  {
    std::ofstream fstream;
    fstream.open(path.c_str(), std::ios::out | std::ios::binary);
    
    io::write(fstream, std::string(80, 0));
    io::write(fstream, static_cast<unsigned long>(mTriangles.size()));

    const short attrCount(0);
    for (Triangles::const_iterator it(mTriangles.begin()); it != mTriangles.end(); ++it)
    {
      io::write(fstream, it->normal());

      io::write(fstream, it->v0());
      io::write(fstream, it->v1());
      io::write(fstream, it->v2());

      io::write(fstream, attrCount);
    }
    fstream.close();
  }
private:
  std::vector<Triangle> mTriangles;
};

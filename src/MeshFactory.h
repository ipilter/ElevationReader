#pragma once

#include <iostream>

#include "GeoReference.h"
#include "Pixel.h"
#include "Elevation.h"
#include "STLFileWriter.h"

// iTODO finish it

struct MeshType
{
  enum Enum
  {
    A
    , B
  };
};

inline Vec2f projectToFlatEarth(const Geo& geo)
{
  static const double scaleX(1852.0f * 60.0f);
  static const double scaleY(1852.0f * 60.0f);
  return Vec2f(static_cast<float>(geo.lon * scaleX),
    static_cast<float>(geo.lat * scaleY));
}

inline MeshType::Enum calculateMeshType(const Vec3f& v00, const Vec3f& v10, const Vec3f& v01, const Vec3f& v11)
{
  const Vec3f n1(glm::cross((v00 - v01), (v00 - v11)));
  const Vec3f n2(glm::cross((v00 - v11), (v00 - v10)));
  const Vec3f n((n1.x + n2.x) / 2.0f,
    (n1.y + n2.y) / 2.0f,
    (n1.z + n2.z) / 2.0f);
  return (n.x > 0.0f && n.y > 0.0f) || (n.x < 0.0f && n.y < 0.0f) ? MeshType::A : MeshType::B;
}

inline void createMesh(const std::string& meshPath
                       , const GeoReference& geoReference
                       , const Elevation::Ptr& elevation
                       , const double elevationScale
                       , const Pixel& start
                       , const Pixel& end
                       , const bool validateValues
                       , const bool useMeshType)
{
  std::cout << "Creating mesh from elevation..." << std::endl;

  const size_t total(elevation->getSize().x * elevation->getSize().y);

  // move mesh origin to 0.0 and let the cordinates be all positive // iTODO: height not proper yet
  const Pixel imgMeshOrigin(start.x, start.y + elevation->getSize().y - 1); // last y pixel at the origo (flipped y in geoTiff)
  const Geo geoMeshOrigin(geoReference.imgToGeo(imgMeshOrigin));

  STLFileWriter stl(total * 2);
  double current(0.0);
  for (int y(0); y < elevation->getSize().y - 1; ++y)
  {
    for (int x(0); x < elevation->getSize().x - 1; ++x)
    {
      const Pixel pix00(start.x + x, start.y + y);
      const Pixel pix10(start.x + x + 1, start.y + y);
      const Pixel pix01(start.x + x, start.y + y + 1);
      const Pixel pix11(start.x + x + 1, start.y + y + 1);

      const Geo geoPix00(geoReference.imgToGeo(pix00) - geoMeshOrigin);
      const Geo geoPix10(geoReference.imgToGeo(pix10) - geoMeshOrigin);
      const Geo geoPix01(geoReference.imgToGeo(pix01) - geoMeshOrigin);
      const Geo geoPix11(geoReference.imgToGeo(pix11) - geoMeshOrigin);

      const Vec3f world00(projectToFlatEarth(geoPix00), static_cast<float>(elevation->getElevation(x, y) * elevationScale));
      const Vec3f world10(projectToFlatEarth(geoPix10), static_cast<float>(elevation->getElevation(x + 1, y) * elevationScale));
      const Vec3f world01(projectToFlatEarth(geoPix01), static_cast<float>(elevation->getElevation(x, y + 1) * elevationScale));
      const Vec3f world11(projectToFlatEarth(geoPix11), static_cast<float>(elevation->getElevation(x + 1, y + 1) * elevationScale));

      if (useMeshType)
      {
        if (calculateMeshType(world00, world10, world01, world11) == MeshType::A)
        {
          stl.addTriangle(Triangle(world00, world10, world01));
          stl.addTriangle(Triangle(world01, world10, world11));
        }
        else
        {
          stl.addTriangle(Triangle(world00, world10, world11));
          stl.addTriangle(Triangle(world11, world01, world00));
        }
      }
      else
      {
        stl.addTriangle(Triangle(world00, world01, world10));
        stl.addTriangle(Triangle(world10, world01, world11));
      }

      ++current;
    }

    std::cout << (current / total) * 100.0 << "% processed...       \r";
  }

  std::cout << "Writting mesh into STL file..." << std::endl;
  stl.write(meshPath);
  std::cout << "Terrain mesh written to " << meshPath << std::endl;
}

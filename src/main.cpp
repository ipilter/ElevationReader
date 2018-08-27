#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <memory>

#include "GDalWrapper.h"
#include "GeoReference.h"
#include "Numbers.h"
#include "Pixel.h"
#include "Tile.h"
#include "GeoCellFactory.h"
#include "Triangle.h"
#include "Ppm.h"
#include "MeshFactory.h"

void saveElevationAsPpm(const Elevation::Ptr& elevation, const std::string& path)
{
  Ppm ppm(elevation->getSize().x, elevation->getSize().y);
  for (int y(0); y < elevation->getSize().y; ++y)
  {
    for (int x(0); x < elevation->getSize().x; ++x)
    {
      Ppm::Pixel::ChannelType gray(elevation->getElevation(x, y));
      ppm.setPixel(x, y, Ppm::Pixel(gray, gray, gray));
    }
  }
  ppm.write(path);
}

// dsmRoot top left bottom right - lat lon lat lon extent format for easy copy-paste from maps.google.com
int main(int argc, char* argv[])
{
  try
  {
    if(argc < 6)
    {
      throw std::runtime_error("missing parameter! Usage: dsmRoot top left bottom right");
    }

    // initialize GDAL
    GDALRegister_GTiff();
    CPLSetErrorHandler(CPLQuietErrorHandler);

    const GeoCellFactory geoCellFactory(argv[1]);
    const Extent inputExtent(parseNumber<double>(argv[3]), parseNumber<double>(argv[2]), parseNumber<double>(argv[5]), parseNumber<double>(argv[4]));

    // calculate the tile(s) for this extent
    const int xMin(Round(inputExtent.leftTop().lon));      // left bottom corner of the extent is the minimum geocell coordinate
    const int yMin(Round(inputExtent.rightBottom().lat));

    const int xMax(Round(inputExtent.rightBottom().lon));  // right top corner of the extent is the maximum geocell coordinate
    const int yMax(Round(inputExtent.leftTop().lat));

    // create geocells
    GeoCell::Vector geoCells;
    for (int y = yMin; y <= yMax; ++y)
    {
      for (int x = xMin; x <= xMax; ++x)
      {
        geoCells.push_back(geoCellFactory.create(x, y));
      }
    }

    // create tiles
    Tile::Vector tiles;
    for (GeoCell::Vector::const_iterator it(geoCells.begin()); it != geoCells.end(); ++it)
    {
      const GeoCell::Ptr& geoCell((*it));

      // create tile with the intersection of the input extent and this geocell
      tiles.push_back(Tile::Ptr(new Tile(geoCell, inputExtent.intersect(geoCell->geoReference()->extent()))));

      const Pixel imgLeftTop(tiles.back()->geoCell()->geoReference()->geoToImg(tiles.back()->extent().leftTop()));
      const Pixel imgRightBottom(tiles.back()->geoCell()->geoReference()->geoToImg(tiles.back()->extent().rightBottom()));

      Elevation::Ptr elevation(new Elevation());
      geoCellFactory.readElevation(tiles.back()->geoCell(), *elevation, imgLeftTop, imgRightBottom);
      tiles.back()->setElevation(elevation);

      // debug input geotiff
      //{
      //  std::stringstream ss;
      //  ss << "d:\\test\\" << geoCell->asString() << ".ppm";
      //  const Geo& geoTiffLeftTop((*it)->geoReference()->extent().leftTop());
      //  const Geo& geoTiffRightBottom((*it)->geoReference()->extent().rightBottom());
      //  const Pixel imgLeftTop(geoCell->geoReference()->geoToImg(geoTiffLeftTop));
      //  const Pixel imgRightBottom(geoCell->geoReference()->geoToImg(geoTiffRightBottom));
      //  Elevation elevation;
      //  geoCellFactory.readElevation(geoCell, elevation, imgLeftTop, imgRightBottom);
      //  saveElevationAsPpm(elevation, ss.str());
      //}
    }

    // post process elevation(s)
    // - solve tile connection artifacts
    // - filter out fake peaks

    // create output
    std::cout << "Input extent = " << inputExtent << std::endl;
    for (Tile::Vector::const_iterator it(tiles.begin()); it != tiles.end(); ++it)
    {
      const Pixel imgLeftTop((*it)->geoCell()->geoReference()->geoToImg((*it)->extent().leftTop()));
      const Pixel imgRightBottom((*it)->geoCell()->geoReference()->geoToImg((*it)->extent().rightBottom()));

      std::cout << "Tile: geocell = " << *(*it)->geoCell() << " Extent: " << (*it)->extent() << std::endl;

      std::cout << "      left top pixel = " << imgLeftTop.x << ", " << imgLeftTop.y << std::endl;
      std::cout << "      right bottom pixel = " << imgRightBottom.x << ", " << imgRightBottom.y << std::endl;

      std::stringstream ss;
      ss << "d:\\test\\" << (*it)->geoCell()->asString();

      saveElevationAsPpm((*it)->elevation(), ss.str() + "-mesh.ppm");

      createMesh(ss.str() + ".stl", *(*it)->geoCell()->geoReference(), (*it)->elevation(), 1.0, imgLeftTop, imgRightBottom, true, true);
    }

    std::cout << "Done." << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
  return 0;
}

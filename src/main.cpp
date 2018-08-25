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

//inputExtent -> tile(s) -> geocell -> georeference -> geotif

// dsmRoot  lat       lon       lat       lon
// d:\dsm   46.840104 17.482264 46.820836 17.513521
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

    const std::string dsmRoot(argv[1]);

    // input is lat-lon but we store lon-lat(x, y) order
    const Extent inputExtent(parseNumber<double>(argv[3]), parseNumber<double>(argv[2]), parseNumber<double>(argv[5]), parseNumber<double>(argv[4]));

    std::cout << "Input extent = " << inputExtent << std::endl;

    // calculate the tile(s) for this extent
    const int xMin(Round(inputExtent.leftTop().lon));      // bottom left corner of the extent is the minimum geocell coordinate
    const int yMin(Round(inputExtent.rightBottom().lat));

    const int xMax(Round(inputExtent.rightBottom().lon));  // top right  corner of the extent is the maximum geocell coordinate
    const int yMax(Round(inputExtent.leftTop().lat));

    GeoCell::Vector geoCells;
    {
      GeoCellFactory geoCellFactory(dsmRoot);
      for (int y = yMin; y <= yMax; ++y)
      {
        for (int x = xMin; x <= xMax; ++x)
        {
          geoCells.push_back(geoCellFactory.create(x, y));
        }
      }
    }

    Tile::Vector tiles;
    for (GeoCell::Vector::const_iterator it(geoCells.begin()); it != geoCells.end(); ++it)
    {
      const GeoCell::Ptr& pGeoCell((*it));

      // calculate the intersection of the input extent and this tile
      const Extent tileExtent(inputExtent.intersect(pGeoCell->geoReference()->extent()));
      tiles.push_back(Tile::Ptr(new Tile(pGeoCell, tileExtent)));
    }

    for (Tile::Vector::const_iterator it(tiles.begin()); it != tiles.end(); ++it)
    {
      const Pixel imgLeftTop((*it)->geoCell()->geoReference()->geoToImg((*it)->extent().leftTop()));
      const Pixel imgRightBottom((*it)->geoCell()->geoReference()->geoToImg((*it)->extent().rightBottom()));

      std::cout << "Tile: geocell = " << *(*it)->geoCell() << " Extent: " << (*it)->extent() << std::endl;

      std::cout << "      left top pixel = " << imgLeftTop.x << ", " << imgLeftTop.y << std::endl;
      std::cout << "      right bottom pixel = " << imgRightBottom.x << ", " << imgRightBottom.y << std::endl;
    }

  // For all tile in tiles
  //  Open the tif image corresponding to the given geocell
  //  Read elevation data from the image inside the tile`s extent
    std::cout << "Done." << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
  return 0;
}

//std::cout << "GeoTiff boundaries" << std::endl
//          << std::fixed << "  left: " << geoReference.extent().left() << std::endl
//          << std::fixed << "  top: " << geoReference.extent().top() << std::endl
//          << std::fixed << "  right: " << geoReference.extent().right() << std::endl
//          << std::fixed << "  bottom: " << geoReference.extent().bottom() << std::endl;

//std::cout << "GeoTiff pixels" << std::endl
//          << "  top left: " << geoReference.geoToImg(geoReference.extent().topLeft()) << std::endl
//          << "  top right: " << geoReference.geoToImg(Geo(geoReference.extent().right(), geoReference.extent().top())) << std::endl
//          << "  bottom left: " << geoReference.geoToImg(Geo(geoReference.extent().left(), geoReference.extent().bottom())) << std::endl
//          << "  bottom right: " << geoReference.geoToImg(geoReference.extent().bottomRight()) << std::endl;

//std::cout << "Input region pixels" << std::endl
//          << "  top left: " << geoReference.geoToImg(inputExtent.topLeft()) << std::endl
//          << "  top right: " << geoReference.geoToImg(Geo(inputExtent.right(), inputExtent.top())) << std::endl
//          << "  bottom left: " << geoReference.geoToImg(Geo(inputExtent.left(), inputExtent.bottom())) << std::endl
//          << "  bottom right: " << geoReference.geoToImg(inputExtent.bottomRight()) << std::endl;


#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <gdal_priv.h>

#include "GeoReference.h"
#include "GeoCell.h"

template<class T> T parseNumber(const std::string& str);
GeoReference getGeoReference(const std::string& geoTiffPath);

class Tile
{
public:
  Tile(const GeoCell& geoCell, const Extent& extent)
    : mGeoCell(geoCell)
    , mExtent(extent)
  {}
public:
  const GeoCell& geoCell() const
  {
    return mGeoCell;
  }
  const Extent& extent() const
  {
    return mExtent;
  }
private:
  GeoCell mGeoCell;
  Extent mExtent;
};

typedef std::vector<Tile> TileVector;

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

    // iTODO now it is using the input region`s left bottom coordinate to figure out the geocell but this is not correct.
    // all the four corner of the input must be checked for geocell due to all four might contained in a different geocell
    // the input in one geocell only if all the four corners in the same geocell..

    // calculate the tile(s) for this extent
    const int xMin(Round(inputExtent.topLeft().lon));      // bottom left corner of the extent is the minimum geocell coordinate
    const int yMin(Round(inputExtent.bottomRight().lat));

    const int xMax(Round(inputExtent.bottomRight().lon));  // top right  corner of the extent is the maximum geocell coordinate
    const int yMax(Round(inputExtent.topLeft().lat));

    TileVector tiles;
    for (int y = yMin; y <= yMax; ++y)
    { 
      for (int x = xMin; x <= xMax; ++x)
      {
        // calculate the intersection if the input extent and this tile
        GeoCell geoCell(x, y);
        Extent extent(0.0, 1.0, 1.0, 0.0)

        tiles.push_back(Tile(geoCell, extent));
      }
    }

    for (TileVector::const_iterator it(tiles.begin()); it != tiles.end(); ++it)
    {
      std::cout << "Tile: geocell = " << it->geoCell() << " Extent: " << it->extent() << std::endl;
    }

	// For all tile in tiles
	//  Open the tif image corresponding to the given geocell
	//  Read elevation data from the image inside the tile`s extent

    //const std::string dsmPath(dsmRoot + "\\" + geocell.asString() + "_AVE_DSM.tif");

    //std::cout << "Reading file " << dsmPath << std::endl;
    //const GeoReference geoReference(getGeoReference(dsmPath));

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

    std::cout << "Done." << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
  return 0;
}

GeoReference getGeoReference(const std::string& geoTiffPath)
{
  GDALDataset* pTiffDataSet((GDALDataset*)GDALOpen(geoTiffPath.c_str(), static_cast<GDALAccess>(GA_ReadOnly)));
  if (!pTiffDataSet)
  {
    throw std::runtime_error(std::string("could not open file: ") + geoTiffPath);
  }

  const int width(pTiffDataSet->GetRasterXSize());
  const int height(pTiffDataSet->GetRasterYSize());

  double geoTransform[6] = { 0 };
  if (pTiffDataSet->GetGeoTransform(geoTransform) != CE_None)
  {
    throw std::runtime_error(std::string("could not get geo transform from image"));
  }
  GDALClose(pTiffDataSet);
  pTiffDataSet = NULL;

  return GeoReference(geoTransform, width, height);
}

template<class T>
T parseNumber(const std::string& str)
{
  T t(0);
  if (!(std::istringstream(str) >> t))
  {
    throw std::runtime_error(std::string("cannot parse ") + str + " as number");
  }
  return t;
}

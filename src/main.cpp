#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <memory>

#include <gdal_priv.h>

#include "GeoReference.h"
#include "GeoCell.h"

template<class T> T parseNumber(const std::string& str);
GeoReference::Ptr createGeoReference(const std::string& geoTiffPath);

//inputExtent -> tile(s) -> geocell -> georeference -> geotif

class Tile
{
public:
  typedef std::shared_ptr<Tile> Ptr;
  typedef std::vector<Ptr> Vector;
public:
  Tile(const GeoCell::Ptr& geoCell, const Extent& extent)
    : mGeoCell(geoCell)
    , mExtent(extent)
  { }
  ~Tile()
  { }
public:
  const GeoCell::Ptr geoCell() const
  {
    return mGeoCell;
  }
  const Extent& extent() const
  {
    return mExtent;
  }
private:
  GeoCell::Ptr mGeoCell;
  Extent mExtent;
};

class GeoCellFactory
{
public:
  GeoCellFactory(const std::string& dsmRoot)
    : mDsmRoot(createRoot(dsmRoot))
    , mDsmEnd("_AVE_DSM")
    , mDsmExtension(".tif")
  {}
public:
  GeoCell::Ptr create(int x, int y)
  {
    GeoCell::Ptr geoCell(new GeoCell(x, y));

    const std::string dsmPath(mDsmRoot + geoCell->asString() + mDsmEnd + mDsmExtension);
    std::cout << "Reading file " << dsmPath << std::endl;

    geoCell->setGeoReference(createGeoReference(dsmPath));

    std::cout << "created " << *geoCell << std::endl;

    return geoCell;
  }
private:
  std::string createRoot(const std::string& dsmRoot)
  {
    std::string root(dsmRoot);
    if (root[root.size() - 1] == '\\')
    {
      return dsmRoot;
    }
    return root += "\\";
  }
private:
  const std::string mDsmRoot;
  const std::string mDsmEnd;
  const std::string mDsmExtension;
};

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
    const int xMin(Round(inputExtent.topLeft().lon));      // bottom left corner of the extent is the minimum geocell coordinate
    const int yMin(Round(inputExtent.bottomRight().lat));

    const int xMax(Round(inputExtent.bottomRight().lon));  // top right  corner of the extent is the maximum geocell coordinate
    const int yMax(Round(inputExtent.topLeft().lat));

    GeoCell::Vector geoCells;
    {
      GeoCellFactory geoCellFactory(dsmRoot);
      for (int y = yMin; y <= yMax; ++y)
      {
        for (int x = xMin; x <= xMax; ++x)
        {
          GeoCell::Ptr geocellPtr = geoCellFactory.create(x, y);  // Geocell bottom left corner
          geoCells.push_back(geocellPtr);
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
      std::cout << "Tile: geocell = " << *(*it)->geoCell() << " Extent: " << (*it)->extent() << std::endl;
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

GeoReference::Ptr createGeoReference(const std::string& geoTiffPath)
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

  return GeoReference::Ptr(new GeoReference(geoTransform, width, height));
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


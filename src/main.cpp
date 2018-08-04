#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <gdal_priv.h>

#include "GeoReference.h"
#include "GeoCell.h"

template<class T> T parseNumber(const std::string& str);
GeoReference getGeoReference(const std::string& geoTiffPath);

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
    
    std::cout << "top left = " << inputExtent.topLeft() << std::endl;
    std::cout << "bottom right = " << inputExtent.bottomRight() << std::endl;


    // iTODO now it is using the input region`s left bottom coordinate to figure out the geocell but this is not correct.
    // all the four corner of the input must be checked for geocell due to all four might contained in a different geocell
    // the input in one geocell only if all the four corners in the same geocell..
    const GeoCell geocell(static_cast<int>(std::floor(inputExtent.topLeft().lon)) // calculate the geocell for this extent
                          , static_cast<int>(std::floor(inputExtent.bottomRight().lat)));

    std::cout << "geocell = " << geocell << std::endl;

    const std::string dsmPath(dsmRoot + "\\" + geocell.asString() + "_AVE_DSM.tif");

    std::cout << "Reading file " << dsmPath << std::endl;
    const GeoReference geoReference(getGeoReference(dsmPath));

    std::cout << "GeoTiff boundaries" << std::endl
              << std::fixed << "  left: " << geoReference.extent().left() << std::endl
              << std::fixed << "  top: " << geoReference.extent().top() << std::endl
              << std::fixed << "  right: " << geoReference.extent().right() << std::endl
              << std::fixed << "  bottom: " << geoReference.extent().bottom() << std::endl;

    std::cout << "GeoTiff pixels" << std::endl
              << "  top left: " << geoReference.geoToImg(geoReference.extent().topLeft()) << std::endl
              << "  top right: " << geoReference.geoToImg(Geo(geoReference.extent().right(), geoReference.extent().top())) << std::endl
              << "  bottom left: " << geoReference.geoToImg(Geo(geoReference.extent().left(), geoReference.extent().bottom())) << std::endl
              << "  bottom right: " << geoReference.geoToImg(geoReference.extent().bottomRight()) << std::endl;

    std::cout << "Input region pixels" << std::endl
              << "  top left: " << geoReference.geoToImg(inputExtent.topLeft()) << std::endl
              << "  top right: " << geoReference.geoToImg(Geo(inputExtent.right(), inputExtent.top())) << std::endl
              << "  bottom left: " << geoReference.geoToImg(Geo(inputExtent.left(), inputExtent.bottom())) << std::endl
              << "  bottom right: " << geoReference.geoToImg(inputExtent.bottomRight()) << std::endl;

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
    GDALDataset* pTiffDataSet = (GDALDataset*)GDALOpen(geoTiffPath.c_str(), static_cast<GDALAccess>(GA_ReadOnly));
    if (!pTiffDataSet)
    {
      throw std::runtime_error(std::string("could not open file: ") + geoTiffPath);
    }

    const int width = pTiffDataSet->GetRasterXSize();
    const int height = pTiffDataSet->GetRasterYSize();

    double geoTransform[6] = { 0 };
    if (pTiffDataSet->GetGeoTransform(geoTransform) != CE_None)
    {
      throw std::runtime_error(std::string("could not get geo transform from image"));
    }
    GDALClose(pTiffDataSet);
    pTiffDataSet = NULL;

    double invGeoTransform[6] = { 0 };
    GDALInvGeoTransform(geoTransform, invGeoTransform);

    return GeoReference(geoTransform, invGeoTransform, width, height);
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

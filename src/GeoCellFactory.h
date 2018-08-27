#pragma once

#include <string>

#include "GDalWrapper.h"
#include "GeoCell.h"
#include "Elevation.h"

class GeoCellFactory
{
public:
  GeoCellFactory(const std::string& dsmRoot)
    : mDsmRoot(createRoot(dsmRoot))
    , mDsmEnd("_AVE_DSM")
    , mDsmExtension(".tif")
  {}
public:
  GeoCell::Ptr create(int x, int y) const
  {
    GeoCell::Ptr geoCell(new GeoCell(x, y));
    geoCell->setGeoReference(createGeoReference(mDsmRoot + geoCell->asString() + mDsmEnd + mDsmExtension));
    return geoCell;
  }
  void readElevation(GeoCell::Ptr geoCell
                     , Elevation& elevation
                     , const Pixel& start
                     , const Pixel& end) const
  {
    std::cout << "Reading elevation data from image..." << std::endl;

    GDALDataset* pTiffDataSet(openGeoTiff(mDsmRoot + geoCell->asString() + mDsmEnd + mDsmExtension));
    readData(getElevationChannel(pTiffDataSet), elevation, start, end);
    GDALClose(pTiffDataSet);
  }
private:
  GeoReference::Ptr createGeoReference(const std::string& geoTiffPath) const
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
  GDALRasterBand* getElevationChannel(GDALDataset* pTiffDataSet) const
  {
    if (pTiffDataSet->GetRasterCount() != 1)
    {
      throw std::runtime_error(std::string("invalid channel count found in image. Only images with one channel supported."));
    }

    GDALRasterBand* pRasterBand(pTiffDataSet->GetRasterBand(1));
    if (!pRasterBand)
    {
      throw std::runtime_error(std::string("could not get channel 1."));
    }
    return pRasterBand;
  }
  GDALDataset* openGeoTiff(const std::string& tiffPath) const
  {
    GDALDataset* pTiffDataSet(static_cast<GDALDataset*>(GDALOpen(tiffPath.c_str(), static_cast<GDALAccess>(GA_ReadOnly))));
    if (!pTiffDataSet)
    {
      throw std::runtime_error(std::string("could not open file: ") + tiffPath + " for reading.");
    }
    return pTiffDataSet;
  }
  void readData(GDALRasterBand* pRasterBand
                , Elevation& elevation
                , const Pixel& start
                , const Pixel& end) const
  {
    const GDALDataType dataType(GDALGetRasterDataType(pRasterBand));

    void* value(NULL);
    switch (dataType)
    {
    case GDT_Int16:
    {
      value = new short;
      break;
    }
    default:
      throw std::runtime_error(std::string("not supported data type in TIFF channel. Only GDT_Int16 supported."));
    }

    elevation.resize(end.x - start.x, end.y - start.y);
    for (int y(0); y < elevation.getSize().y; ++y)
    {
      for (int x(0); x < elevation.getSize().x; ++x)
      {
        if (pRasterBand->RasterIO(GF_Read, start.x + x, start.y + y, 1, 1, value, 1, 1, dataType, 0, 0) != CE_None) // more than 1x1 reading much better
        {
          elevation.setElevation(x, y, Elevation::getNoDataValue());
          continue;
        }
        elevation.setElevation(x, y, *static_cast<short*>(value));
      }
    }

    delete value;
  }
  std::string createRoot(const std::string& dsmRoot) const
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

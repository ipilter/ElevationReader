#pragma once

#include <string>

#include "GDalWrapper.h"
#include "GeoCell.h"

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
    geoCell->setGeoReference(createGeoReference(mDsmRoot + geoCell->asString() + mDsmEnd + mDsmExtension));
    return geoCell;
  }
private:
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

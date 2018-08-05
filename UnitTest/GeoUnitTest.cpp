#include <gtest/gtest.h>
#include <vector>

#include <Geo.h>
#include <Types.h>
typedef std::vector<Vec2> GeoCoordinateList;

TEST(testGeo, constructionSuccessTest)
{
  GeoCoordinateList geoCoordinateList;

  geoCoordinateList.push_back(Vec2(180.0, 90.0));
  geoCoordinateList.push_back(Vec2(-180.0, 90.0));
  geoCoordinateList.push_back(Vec2(180.0, -90.0));
  geoCoordinateList.push_back(Vec2(-180.0, -90.0));

  for (size_t i(0); i < geoCoordinateList.size(); ++i)
  {
    Geo geo(geoCoordinateList[i].x, geoCoordinateList[i].y);
    EXPECT_NO_THROW(geo.validate());
  }
}

TEST(testGeo, constructionFailTest)
{
  GeoCoordinateList geoCoordinateList;

  geoCoordinateList.push_back(Vec2(180.1, 0));
  geoCoordinateList.push_back(Vec2(-180.1, 0));

  geoCoordinateList.push_back(Vec2(180.001, 0));
  geoCoordinateList.push_back(Vec2(-180.001, 0));

  geoCoordinateList.push_back(Vec2(180.00001, 0));
  geoCoordinateList.push_back(Vec2(-180.00001, 0));

  geoCoordinateList.push_back(Vec2(180.0 + 0.00001, 0));
  geoCoordinateList.push_back(Vec2(-180.0 - 0.00001, 0));

  geoCoordinateList.push_back(Vec2(180.0 + 0.0000000000001, 0));
  geoCoordinateList.push_back(Vec2(-180.0 - 0.0000000000001, 0));

  geoCoordinateList.push_back(Vec2(180.1, 90.0));
  geoCoordinateList.push_back(Vec2(180.0, 90.1));
  geoCoordinateList.push_back(Vec2(180.1, 90.1));

  for (size_t i(0); i < geoCoordinateList.size(); ++i)
  {
    Geo geo(geoCoordinateList[i].x, geoCoordinateList[i].y);
    EXPECT_THROW(geo.validate(), std::runtime_error);
  }
}

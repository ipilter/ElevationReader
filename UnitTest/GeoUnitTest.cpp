#include <gtest/gtest.h>
#include <vector>

#include <Geo.h>
#include <Types.h>
typedef std::vector<Geo> GeoCoordinateList;

TEST(testGeo, constructionSuccessTest)
{
  GeoCoordinateList geoCoordinateList;

  geoCoordinateList.push_back(Geo(180.0, 90.0));
  geoCoordinateList.push_back(Geo(-180.0, 90.0));
  geoCoordinateList.push_back(Geo(180.0, -90.0));
  geoCoordinateList.push_back(Geo(-180.0, -90.0));

  for (size_t i(0); i < geoCoordinateList.size(); ++i)
  {
    Geo geo(geoCoordinateList[i].lon, geoCoordinateList[i].lat);
    EXPECT_NO_THROW(geo.validate());
  }
}

TEST(testGeo, constructionFailTest)
{
  GeoCoordinateList geoCoordinateList;

  geoCoordinateList.push_back(Geo(180.1, 0));
  geoCoordinateList.push_back(Geo(-180.1, 0));

  geoCoordinateList.push_back(Geo(180.001, 0));
  geoCoordinateList.push_back(Geo(-180.001, 0));

  geoCoordinateList.push_back(Geo(180.00001, 0));
  geoCoordinateList.push_back(Geo(-180.00001, 0));

  geoCoordinateList.push_back(Geo(180.0 + 0.00001, 0));
  geoCoordinateList.push_back(Geo(-180.0 - 0.00001, 0));

  geoCoordinateList.push_back(Geo(180.0 + 0.0000000000001, 0));
  geoCoordinateList.push_back(Geo(-180.0 - 0.0000000000001, 0));

  geoCoordinateList.push_back(Geo(180.1, 90.0));
  geoCoordinateList.push_back(Geo(180.0, 90.1));
  geoCoordinateList.push_back(Geo(180.1, 90.1));

  for (size_t i(0); i < geoCoordinateList.size(); ++i)
  {
    Geo geo(geoCoordinateList[i].lon, geoCoordinateList[i].lat);
    EXPECT_THROW(geo.validate(), std::runtime_error);
  }
}

TEST(testGeo, lessOperatorTest)
{
  GeoCoordinateList geoCoordinateList;

  geoCoordinateList.push_back(Geo(0.0, 0.0));
  geoCoordinateList.push_back(Geo(1.0, 0.0));
  geoCoordinateList.push_back(Geo(0.0, 1.0));
  geoCoordinateList.push_back(Geo(1.0, 1.0));

  EXPECT_TRUE(geoCoordinateList[0] < geoCoordinateList[1]);
  EXPECT_TRUE(geoCoordinateList[0] < geoCoordinateList[2]);
  EXPECT_TRUE(geoCoordinateList[0] < geoCoordinateList[3]);
  EXPECT_TRUE(geoCoordinateList[2] < geoCoordinateList[1]);
  EXPECT_TRUE(geoCoordinateList[1] < geoCoordinateList[3]);
  EXPECT_TRUE(geoCoordinateList[2] < geoCoordinateList[3]);

  EXPECT_FALSE(geoCoordinateList[3] < geoCoordinateList[2]);
  EXPECT_FALSE(geoCoordinateList[1] < geoCoordinateList[2]);
  EXPECT_FALSE(geoCoordinateList[3] < geoCoordinateList[1]);
  EXPECT_FALSE(geoCoordinateList[3] < geoCoordinateList[0]);
  EXPECT_FALSE(geoCoordinateList[2] < geoCoordinateList[0]);
  EXPECT_FALSE(geoCoordinateList[1] < geoCoordinateList[0]);
  EXPECT_FALSE(geoCoordinateList[3] < geoCoordinateList[3]);
}

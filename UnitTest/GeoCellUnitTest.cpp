#include <gtest/gtest.h>
#include <vector>
#include <GeoCell.h>

typedef std::vector<std::string> ExpectedList;
typedef std::vector<Vec2i> BottomLeftList;

TEST(testGeoCell, constructionTest)
{
  ExpectedList expecteds;
  BottomLeftList bottomLefts;

  bottomLefts.push_back(Vec2i(0, 0));
  expecteds.push_back("N000E000");
  bottomLefts.push_back(Vec2i(1, 0));
  expecteds.push_back("N000E001");
  bottomLefts.push_back(Vec2i(0, 1));
  expecteds.push_back("N001E000");
  bottomLefts.push_back(Vec2i(1, 1));
  expecteds.push_back("N001E001");

  bottomLefts.push_back(Vec2i(-1, 0));
  expecteds.push_back("N000W001");
  bottomLefts.push_back(Vec2i(0, -1));
  expecteds.push_back("S001E000");
  bottomLefts.push_back(Vec2i(-1, -1));
  expecteds.push_back("S001W001");

  assert(expecteds.size() == bottomLefts.size());
  for (size_t i(0); i < bottomLefts.size(); ++i)
  {
    GeoCell gc(bottomLefts[i].x, bottomLefts[i].y);
    EXPECT_EQ(expecteds[i], gc.asString());
  }
}

TEST(testGeoCell, constructionBorderTest)
{
  BottomLeftList bottomLefts;

  bottomLefts.push_back(Vec2i(179, 0));
  bottomLefts.push_back(Vec2i(-179, 0));
  bottomLefts.push_back(Vec2i(0, 89));
  bottomLefts.push_back(Vec2i(0, -89));

  for (size_t i(0); i < bottomLefts.size(); ++i)
  {
    EXPECT_NO_THROW(GeoCell(bottomLefts[i].x, bottomLefts[i].y));
  }
}

TEST(testGeoCell, constructionFailTest)
{
  BottomLeftList bottomLefts;

  bottomLefts.push_back(Vec2i(180, 0));
  bottomLefts.push_back(Vec2i(-180, 0));
  bottomLefts.push_back(Vec2i(0, 90));
  bottomLefts.push_back(Vec2i(0, -90));

  for (size_t i(0); i < bottomLefts.size(); ++i)
  {
    EXPECT_THROW(GeoCell(bottomLefts[i].x, bottomLefts[i].y), std::runtime_error);
  }
}

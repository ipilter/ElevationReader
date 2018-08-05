#include <gtest/gtest.h>
#include <vector>
#include <Extent.h>

typedef std::pair<Geo, Geo> TopLeftBottomRight;
typedef std::vector<TopLeftBottomRight> TopLeftBottomRightList;

TEST(testExtent, constructionSuccessTest)
{
  TopLeftBottomRightList tlbrList;

  tlbrList.push_back(std::make_pair(Geo(0, 0), Geo(0, 0)));

  for (size_t i(0); i < tlbrList.size(); ++i)
  {
    EXPECT_NO_THROW(Extent(tlbrList[i].first, tlbrList[i].second));
  }
}

TEST(testExtent, constructionFailTest)
{
  TopLeftBottomRightList tlbrList;

  tlbrList.push_back(std::make_pair(Geo(0, -1), Geo(0, 0)));
  tlbrList.push_back(std::make_pair(Geo(0, 0), Geo(-1, 0)));
  tlbrList.push_back(std::make_pair(Geo(0, 0), Geo(0, 1)));
  tlbrList.push_back(std::make_pair(Geo(1, 0), Geo(0, 0)));

  for (size_t i(0); i < tlbrList.size(); ++i)
  {
    EXPECT_THROW(Extent(tlbrList[i].first, tlbrList[i].second), std::runtime_error);
  }
}

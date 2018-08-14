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

TEST(testExtent, nonOverlapingTest)
{
  TopLeftBottomRightList tlbrList;
  tlbrList.push_back(std::make_pair(Geo(0, 1), Geo(1, 0)));
  tlbrList.push_back(std::make_pair(Geo(2, 1), Geo(3, 0)));
  tlbrList.push_back(std::make_pair(Geo(1, 1), Geo(2, 0)));

  const Extent a(tlbrList[0].first, tlbrList[0].second);
  const Extent b(tlbrList[1].first, tlbrList[1].second);
  const Extent c(tlbrList[2].first, tlbrList[2].second);

  EXPECT_FALSE(a.overlap(b));
  EXPECT_FALSE(b.overlap(a));
  EXPECT_FALSE(a.overlap(c));
  EXPECT_FALSE(c.overlap(a));
}

TEST(testExtent, overlapingTest)
{
  TopLeftBottomRightList tlbrList;
  tlbrList.push_back(std::make_pair(Geo(0, 1), Geo(2, 0)));
  tlbrList.push_back(std::make_pair(Geo(1.5, 1), Geo(2.5, 0)));

  const Extent a(tlbrList[0].first, tlbrList[0].second);
  const Extent b(tlbrList[1].first, tlbrList[1].second);

  EXPECT_TRUE(a.overlap(b));
  EXPECT_TRUE(b.overlap(a));
}

TEST(testExtent, intersectTest)
{
  TopLeftBottomRightList tlbrList;
  tlbrList.push_back(std::make_pair(Geo(0, 2), Geo(2, 0)));
  tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

  const Extent expected(1, 2, 2, 1);

  const Extent a(tlbrList[0].first, tlbrList[0].second);
  const Extent b(tlbrList[1].first, tlbrList[1].second);

  const Extent intersection(a.intersect(b));

  EXPECT_EQ(expected, intersection);
}

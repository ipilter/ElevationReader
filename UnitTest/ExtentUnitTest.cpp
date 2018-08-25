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

TEST(testExtent, overlapingTestAInB)
{
  TopLeftBottomRightList tlbrList;
  tlbrList.push_back(std::make_pair(Geo(0, 1), Geo(2, 0)));
  tlbrList.push_back(std::make_pair(Geo(1.5, 1), Geo(2.5, 0)));

  const Extent a(tlbrList[0].first, tlbrList[0].second);
  const Extent b(tlbrList[1].first, tlbrList[1].second);

  EXPECT_TRUE(a.overlap(b));
  EXPECT_TRUE(b.overlap(a));
}

// b inside a
TEST(testExtent, intersectTestAInsideB)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(0, 10), Geo(10, 0)));
    tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

    const Extent expected(1, 3, 3, 1);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);

    const Extent intersection(a.intersect(b));

    EXPECT_EQ(expected, intersection);
  }
}

// b touches a at bottom left
TEST(testExtent, intersectTestBottomLeft)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(0, 2), Geo(2, 0)));
    tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

    const Extent expected(1, 2, 2, 1);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);

    EXPECT_EQ(expected, a.intersect(b));
    EXPECT_EQ(expected, b.intersect(a));
  }
}

// b touches a at bottom right
TEST(testExtent, intersectTestBottomRigth)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(2, 2), Geo(4, 0)));
    tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

    const Extent expected(2, 2, 3, 1);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);

    EXPECT_EQ(expected, a.intersect(b));
    EXPECT_EQ(expected, b.intersect(a));
  }
}

// b touches a at top left
TEST(testExtent, intersectTestTopLeft)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(0, 4), Geo(2, 2)));
    tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

    const Extent expected(1, 3, 2, 2);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);

    EXPECT_EQ(expected, a.intersect(b));
    EXPECT_EQ(expected, b.intersect(a));
  }
}

// b touches a at top right
TEST(testExtent, intersectTestTopRight)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(2, 4), Geo(4, 2)));
    tlbrList.push_back(std::make_pair(Geo(1, 3), Geo(3, 1)));

    const Extent expected(2, 3, 3, 2);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);

    EXPECT_EQ(expected, a.intersect(b));
    EXPECT_EQ(expected, b.intersect(a));
  }
}

// c between a b, a and b touches horizontaly
TEST(testExtent, intersectTestCInAAndBHorizontaly)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(0, 10), Geo(10, 0)));
    tlbrList.push_back(std::make_pair(Geo(0, 0), Geo(10, -10)));
    tlbrList.push_back(std::make_pair(Geo(2, 2), Geo(4, -2)));

    const Extent expected_ac(2, 2, 4, 0);
    const Extent expected_bc(2, 0, 4, -2);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);
    const Extent c(tlbrList[2].first, tlbrList[2].second);

    EXPECT_EQ(expected_ac, a.intersect(c));
    EXPECT_EQ(expected_bc, b.intersect(c));
  }
}

// c between a b, a and b touches verticaly
TEST(testExtent, intersectTestCInAAndBVerticaly)
{
  {
    TopLeftBottomRightList tlbrList;
    tlbrList.push_back(std::make_pair(Geo(-10, 10), Geo(0, 0)));
    tlbrList.push_back(std::make_pair(Geo(0, 10), Geo(10, 0)));
    tlbrList.push_back(std::make_pair(Geo(-2, 4), Geo(2, 2)));

    const Extent expected_ac(-2, 4, 0, 2);
    const Extent expected_bc(0, 4, 2, 2);

    const Extent a(tlbrList[0].first, tlbrList[0].second);
    const Extent b(tlbrList[1].first, tlbrList[1].second);
    const Extent c(tlbrList[2].first, tlbrList[2].second);

    EXPECT_EQ(expected_ac, a.intersect(c));
    EXPECT_EQ(expected_bc, b.intersect(c));
  }
}

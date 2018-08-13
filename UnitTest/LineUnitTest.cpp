#include <gtest/gtest.h>
#include <vector>
#include <list>

#include <Line.h>
#include <Types.h>
#include <Geo.h>

typedef std::vector<Geo> GeoCoordinateArray;
typedef std::vector<Line> LineList;

namespace
{
  inline bool isEven(const size_t& u)
  {
    return (u & 1) == 0;
  }

  LineList createLines(const GeoCoordinateArray& geoCoordinateList)
  {
    if(!isEven(geoCoordinateList.size()))
    { 
      throw std::runtime_error("invalid geoCoordinateList");
    }

    LineList lines;

    GeoCoordinateArray::const_iterator firstIt(geoCoordinateList.begin());
    for (; firstIt != geoCoordinateList.end();)
    {
      GeoCoordinateArray::const_iterator lastIt(firstIt + 1);
      lines.push_back(Line(*firstIt, *lastIt));
      firstIt = lastIt + 1;
    }
    return lines;
  }
}

TEST(testLine, constructionSuccessTest)
{
  GeoCoordinateArray geoCoordinateList;
  geoCoordinateList.push_back(Vec2(0.0, 0.0));
  geoCoordinateList.push_back(Vec2(10.0, 0.0));

  LineList lines(createLines(geoCoordinateList));
  
  GeoCoordinateArray::const_iterator expectedIt(geoCoordinateList.begin());
  for (LineList::const_iterator it(lines.begin()); it != lines.end(); ++it)
  {
    EXPECT_EQ(*expectedIt, it->leftTop());
    EXPECT_EQ(*(++expectedIt), it->rightBottom());
  }
}

TEST(testLine, intersectSuccessTest)
{
  GeoCoordinateArray geoCoordinateList;
  geoCoordinateList.push_back(Vec2(-10.0, 0.0));
  geoCoordinateList.push_back(Vec2(10.0, 0.0));
  geoCoordinateList.push_back(Vec2(0.0, -10.0));
  geoCoordinateList.push_back(Vec2(0.0, 10.0));

  LineList lines(createLines(geoCoordinateList));
  if (!isEven(lines.size()))
  {
    throw std::runtime_error("invalid geoCoordinateList");
  }

  LineList::const_iterator firstIt(lines.begin());
  for (; firstIt != lines.end();)
  {
    LineList::const_iterator lastIt(firstIt + 1);
    EXPECT_EQ(Geo(0.0, 0.0), firstIt->intersect(*lastIt));
    firstIt = lastIt + 1;
  }
}

TEST(testLine, intersectFailTest)
{
  GeoCoordinateArray geoCoordinateList;
  geoCoordinateList.push_back(Vec2(-10.0, 0.0));
  geoCoordinateList.push_back(Vec2(10.0, 0.0));
  geoCoordinateList.push_back(Vec2(-10.0, 1.0));
  geoCoordinateList.push_back(Vec2(10.0, 1.0));

  geoCoordinateList.push_back(Vec2(0.0, -10.0));
  geoCoordinateList.push_back(Vec2(0.0, 10.0));
  geoCoordinateList.push_back(Vec2(1.0, -10.0));
  geoCoordinateList.push_back(Vec2(1.0, 10.0));


  LineList lines(createLines(geoCoordinateList));
  if (!isEven(lines.size()))
  {
    throw std::runtime_error("invalid geoCoordinateList");
  }

  LineList::const_iterator firstIt(lines.begin());
  for (; firstIt != lines.end();)
  {
    LineList::const_iterator lastIt(firstIt + 1);
    EXPECT_EQ(Geo::invalid(), firstIt->intersect(*lastIt));
    firstIt = lastIt + 1;
  }
}
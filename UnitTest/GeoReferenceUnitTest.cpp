#include <gtest/gtest.h>
#include <vector>

#include <GeoReference.h>

/*
  Xgeo = GT(0) + Xpixel*GT(1) + Yline*GT(2)
  Ygeo = GT(3) + Xpixel*GT(4) + Yline*GT(5)

  gdal geo transformation
  adfGeoTransform[0] top left x
  adfGeoTransform[1] w-e pixel resolution
  adfGeoTransform[2] 0 
  adfGeoTransform[3] top left y
  adfGeoTransform[4] 0
  adfGeoTransform[5] n-s pixel resolution (negative value)
*/

TEST(testGeoReference, transformTest)
{
  const Vec2 leftTop(0.0, 1.0);
  const Vec2i imageSize(100, 100);

  const Vec2 tileSize(1.0, 1.0);

  const Vec2 imageResolution(tileSize.x / imageSize.x, tileSize.y / imageSize.y);

  double transform[6] = { leftTop.x, imageResolution.x, 0.0, leftTop.y, 0.0, -imageResolution.y };
  GeoReference geoReference(transform, imageSize.x, imageSize.y);

  EXPECT_EQ(Geo(0.0,        tileSize.y), geoReference.imgToGeo(Vec2i(0,           0)));
  EXPECT_EQ(Geo(tileSize.x, tileSize.y), geoReference.imgToGeo(Vec2i(imageSize.x, 0)));
  EXPECT_EQ(Geo(0.0,        0.0),        geoReference.imgToGeo(Vec2i(0,           imageSize.y)));
  EXPECT_EQ(Geo(tileSize.x, 0.0),        geoReference.imgToGeo(Vec2i(imageSize.x, imageSize.y)));
}

TEST(testGeoReference, inverseTransformTest)
{
  const Vec2 leftTop(0.0, 1.0);
  const Vec2i imageSize(100, 100);

  const Vec2 tileSize(1.0, 1.0);

  const Vec2 imageResolution(tileSize.x / imageSize.x, tileSize.y / imageSize.y);

  double transform[6] = { leftTop.x, imageResolution.x, 0.0, leftTop.y, 0.0, -imageResolution.y };
  GeoReference geoReference(transform, imageSize.x, imageSize.y);

  EXPECT_EQ(Geo(tileSize.x, tileSize.y), geoReference.imgToGeo(Vec2i(imageSize.x, 0)));
  EXPECT_EQ(Vec2i(imageSize.x, 0), geoReference.geoToImg(Geo(tileSize.x, tileSize.y)));
}

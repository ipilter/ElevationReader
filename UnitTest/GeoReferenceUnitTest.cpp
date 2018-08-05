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
  const Vec2 topLeft(0.0, 1.0);
  const Vec2i imageSize(100, 100);

  const Vec2 tileSize(1.0, 1.0);

  const Vec2 imageResolution(tileSize.x / imageSize.x, tileSize.y / imageSize.y);

  double transform[6] = { topLeft.x, imageResolution.x, 0.0, topLeft.y, 0.0, -imageResolution.y };
  double inverseTransform[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  GeoReference geoReference(transform, inverseTransform, imageSize.x, imageSize.y);

  EXPECT_EQ(Geo(0.0,        tileSize.y), geoReference.imgToGeo(Vec2i(0,           0)));
  EXPECT_EQ(Geo(tileSize.x, tileSize.y), geoReference.imgToGeo(Vec2i(imageSize.x, 0)));
  EXPECT_EQ(Geo(0.0,        0.0),        geoReference.imgToGeo(Vec2i(0,           imageSize.y)));
  EXPECT_EQ(Geo(tileSize.x, 0.0),        geoReference.imgToGeo(Vec2i(imageSize.x, imageSize.y)));
}

TEST(testGeoReference, inverseTransformTest)
{
  const Vec2 topLeft(0.0, 1.0);
  const Vec2i imageSize(100, 100);

  const Vec2 tileSize(1.0, 1.0);

  const Vec2 imageResolution(tileSize.x / imageSize.x, tileSize.y / imageSize.y);

  double transform[6] = { topLeft.x, imageResolution.x, 0.0, topLeft.y, 0.0, -imageResolution.y };
  /* we assume a 3rd row that is[1 0 0]

    Compute determinate
    det = gt_in[1] * gt_in[5] - gt_in[2] * gt_in[4]

    if (abs(det) < 0.000000000000001) :
      return

      inv_det = 1.0 / det

      # compute adjoint, and divide by determinate
      gt_out = [0, 0, 0, 0, 0, 0]
      gt_out[1] = gt_in[5] * inv_det
      gt_out[4] = -gt_in[4] * inv_det

      gt_out[2] = -gt_in[2] * inv_det
      gt_out[5] = gt_in[1] * inv_det

      gt_out[0] = (gt_in[2] * gt_in[3] - gt_in[0] * gt_in[5]) * inv_det
      gt_out[3] = (-gt_in[1] * gt_in[3] + gt_in[0] * gt_in[4]) * inv_det

      return gt_out
*/

  double inverseTransform[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  GeoReference geoReference(transform, inverseTransform, imageSize.x, imageSize.y);

  EXPECT_EQ(Geo(tileSize.x, tileSize.y), geoReference.imgToGeo(Vec2i(imageSize.x, 0)));
  EXPECT_EQ(Vec2i(imageSize.x, 0), geoReference.geoToImg(Geo(tileSize.x, tileSize.y)));
}

#pragma once

#define Abs(x)    ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

inline double RelDif(double a, double b)
{
  const double c = Abs(a);
  double d = Abs(b);

  d = Max(c, d);

  return d == 0.0 ? 0.0 : Abs(a - b) / d;
}

inline bool Equal(const double a, const double b, const double tolerance = 0.0001)
{
  return RelDif(a, b) <= tolerance;
}

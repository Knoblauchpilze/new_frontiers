#ifndef    POINT_HXX
# define   POINT_HXX

# include "Point.hh"
# include "LocationUtils.hh"

namespace new_frontiers {

  inline
  Point
  newPoint(float x, float y) {
    return Point{x, y};
  }

  inline
  bool
  toDirection(const Point& s,
              const Point& t,
              float& xD,
              float& yD,
              float& d,
              float threshold) noexcept
  {
    // Compute the direction and compute
    // the length and a unit vector from
    // there.
    xD = t.x - s.x;
    yD = t.y - s.y;

    d = distance::d(s, t);
    bool notZeroLength = (d > threshold);

    if (notZeroLength) {
      xD /= d;
      yD /= d;
    }

    return notZeroLength;
  }

}

#endif    /* POINT_HXX */

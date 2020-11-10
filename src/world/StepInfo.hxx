#ifndef    STEP_INFO_HXX
# define   STEP_INFO_HXX

# include "StepInfo.hh"
# include "LocationUtils.hh"

namespace new_frontiers {

  inline
  void
  StepInfo::clampCoord(float& x, float& y) const noexcept {
    x = std::min(std::max(x, xMin), xMax);
    y = std::min(std::max(y, yMin), yMax);
  }

  inline
  void
  StepInfo::clampCoord(Point& p) const noexcept {
    p.x = std::min(std::max(p.x, xMin), xMax);
    p.y = std::min(std::max(p.y, yMin), yMax);
  }

  inline
  void
  StepInfo::clampPath(const Point& s, float& xD, float& yD, float& d) const noexcept {
    // Compute expected final position.
    Point t;
    t.x = s.x + xD * d;
    t.y = s.y + yD * d;

    // Clamp these coordinates.
    clampCoord(t);

    // Reevaluate the direction from there.
    toDirection(s, t, xD, yD, d);
  }

  inline
  bool
  StepInfo::toDirection(const Point& s,
                        const Point& t,
                        float& xD,
                        float& yD,
                        float& d,
                        float threshold) const noexcept
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

#endif    /* STEP_INFO_HXX */

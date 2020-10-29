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
  StepInfo::clampPath(float xS, float yS, float& xD, float& yD, float& d) const noexcept {
    // Compute expected final position.
    float xT = xS + xD * d;
    float yT = yS + yD * d;

    // Clamp these coordinates.
    clampCoord(xT, yT);

    // Reevaluate the direction from there.
    xD = xT - xS;
    yD = yT - yS;

    // Reevaluate the length of the path.
    d = distance::d(xS, yS, xT, yT);
    if (d > 0.0001f) {
      xD /= d;
      yD /= d;
    }
  }

}

#endif    /* STEP_INFO_HXX */

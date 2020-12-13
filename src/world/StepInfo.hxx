#ifndef    STEP_INFO_HXX
# define   STEP_INFO_HXX

# include "StepInfo.hh"
# include <maths_utils/LocationUtils.hh>

namespace new_frontiers {

  inline
  void
  StepInfo::clampCoord(float& x, float& y) const noexcept {
    x = std::min(std::max(x, xMin), xMax);
    y = std::min(std::max(y, yMin), yMax);
  }

  inline
  void
  StepInfo::clampCoord(utils::Point2f& p) const noexcept {
    p.x() = std::min(std::max(p.x(), xMin), xMax);
    p.y() = std::min(std::max(p.y(), yMin), yMax);
  }

  inline
  void
  StepInfo::clampPath(const utils::Point2f& s, float& xD, float& yD, float& d) const noexcept {
    // Compute expected final position.
    utils::Point2f t;
    t.x() = s.x() + xD * d;
    t.y() = s.y() + yD * d;

    // Clamp these coordinates.
    clampCoord(t);

    // Reevaluate the direction from there.
    toDirection(s, t, xD, yD, d);
  }

}

#endif    /* STEP_INFO_HXX */

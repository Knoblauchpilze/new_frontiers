#ifndef    STEP_INFO_HXX
# define   STEP_INFO_HXX

# include "StepInfo.hh"

namespace new_frontiers {

  inline
  void
  StepInfo::clampCoord(float& x, float& y) const noexcept {
    x = std::min(std::max(x, xMin), xMax);
    y = std::min(std::max(y, yMin), yMax);
  }

}

#endif    /* STEP_INFO_HXX */

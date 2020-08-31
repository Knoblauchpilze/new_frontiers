#ifndef    WORLD_ELEMENT_LOCATOR_HXX
# define   WORLD_ELEMENT_LOCATOR_HXX

# include "WorldElementLocator.hh"

namespace new_frontiers {

  inline
  void
  WorldElementLocator::refresh(const Update& /*update*/) {
    /** TODO: Handle this. We probably need some sort of
     * locatorEntry, where the position, the index and
     * the **/
  }

  inline
  float
  WorldElementLocator::d(float x1, float y1, float x2, float y2) const noexcept {
    return std::sqrt(d2(x1, y1, x2, y2));
  }

  inline
  float
  WorldElementLocator::d2(float x1, float y1, float x2, float y2) const noexcept {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  }

}

#endif    /* WORLD_ELEMENT_LOCATOR_HXX */

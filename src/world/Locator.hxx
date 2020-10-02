#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"

namespace new_frontiers {

  inline
  void
  Locator::refresh(const Update& update) {
    // TODO: Handle this. We probably need some sort of
    // locatorEntry, where the position and the index
    // are stored for easy access and then inserted in
    // a quadtree or something allowing easy spatial
    // sorting.
    if (update == Update::Full) {
      m_blocksIDs.clear();

      initialize();
    }
  }

  inline
  float
  Locator::d(float x1, float y1, float x2, float y2) const noexcept {
    return std::sqrt(d2(x1, y1, x2, y2));
  }

  inline
  float
  Locator::d2(float x1, float y1, float x2, float y2) const noexcept {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  }

}

#endif    /* LOCATOR_HXX */

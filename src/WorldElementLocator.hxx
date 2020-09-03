#ifndef    WORLD_ELEMENT_LOCATOR_HXX
# define   WORLD_ELEMENT_LOCATOR_HXX

# include "WorldElementLocator.hh"

namespace new_frontiers {

  inline
  void
  WorldElementLocator::refresh(const Update& update) {
    // TODO: Handle this. We probably need some sort of
    // locatorEntry, where the position and the index
    // are stored for easy access and then inserted in
    // a quadtree or something allowing easy spatial
    // sorting.

    if (update == Update::Full) {
      m_solidIDs.clear();

      initialize();
    }
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

  inline
  bool
  WorldElementLocator::obstructed(float x, float y) const noexcept {
    // Convert to integer coordinates.
    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);

    return m_solidIDs.count(yi * m_w + xi) > 0;
  }

}

#endif    /* WORLD_ELEMENT_LOCATOR_HXX */

#ifndef    LOCATION_UTILS_HXX
# define   LOCATION_UTILS_HXX

# include "LocationUtils.hh"

namespace new_frontiers {
  namespace distance {

    inline
    float
    d(float x1, float y1, float x2, float y2) noexcept {
      return std::sqrt(d2(x1, y1, x2, y2));
    }

    inline
    float
    d2(float x1, float y1, float x2, float y2) noexcept {
      return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

  }
}

#endif    /* LOCATION_UTILS_HXX */

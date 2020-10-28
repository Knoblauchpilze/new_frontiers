#ifndef    PATH_HXX
# define   PATH_HXX

# include "Path.hh"

namespace new_frontiers {
  namespace path {

    inline
    float
    Segment::length() const noexcept {
      return std::sqrt((xT - xO) * (xT - xO) + (yT - yO) * (yT - yO));
    }

  }
}

#endif    /* PATH_HXX */

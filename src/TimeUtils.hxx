#ifndef    TIME_UTILS_HXX
# define   TIME_UTILS_HXX

# include "TimeUtils.hh"

namespace new_frontiers {

  inline
  TimeStamp
  now() noexcept {
    return std::chrono::system_clock::now();
  }

  inline
  Duration
  toMilliseconds(int ms) noexcept {
    return std::chrono::milliseconds(ms);
  }

}

#endif    /* TIME_UTILS_HXX */

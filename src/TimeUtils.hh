#ifndef    TIME_UTILS_HH
# define   TIME_UTILS_HH

# include <chrono>

namespace new_frontiers {

  /**
   * @brief - Convenience define to refer to a time point.
   *          Allows to keep track of absolute timestamps
   *          to trigger some behaviors.
   */
  using TimeStamp = std::chrono::system_clock::time_point;

  /**
   * @brief - Conveniently represent a duration. It can
   *          be added or subtracted to a timestamp in
   *          an easy way.
   */
  using Duration = std::chrono::system_clock::duration;

  /**
   * @brief - Used to retrieve the timestamp as of now.
   * @return - a timepoint at the moment of the call to
   *           this function.
   */
  TimeStamp
  now() noexcept;

  /**
   * @brief - Used to convert the input integer as a time
   *          duration expressed in milliseconds.
   * @param ms - the number of milliseconds to convert.
   * @return - a duration representing the input number of
   *           milliseconds.
   */
  Duration
  toMilliseconds(int ms) noexcept;

}

# include "TimeUtils.hxx"

#endif    /* TIME_UTILS_HH */

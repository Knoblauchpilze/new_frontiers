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
   * @brief - Convenience define to represent a single
   *          millisecond.
   */
  using Milliseconds = std::chrono::milliseconds;

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

  /**
   * @brief - Converts a timestamp to a human readable
   *          string.
   * @param t - the time to convert.
   * @return - a string representing this time.
   */
  std::string
  timeToString(const TimeStamp& t) noexcept;

  /**
   * @brief - Converts the duration to a human readable
   *          string expressed in milliseconds.
   * @param d - the duration to convert to string. Be
   *            aware that it should not be 'too far'
   *            from one millisecond in order to get
   *            a display that is not a huge batch of
   *            numbers.
   * @return - the corresponding string.
   */
  std::string
  durationToString(const Duration& d) noexcept;

  /**
   * @brief - Return the difference in milliseconds between
   *          the two input timestamps using a float value.
   * @param start - the start of the time interval.
   * @param end - the end of the time interval.
   * @return - a float value for the interval in milliseconds.
   */
  float
  diffInMs(const TimeStamp& start, const TimeStamp& end) noexcept;

}

# include "TimeUtils.hxx"

#endif    /* TIME_UTILS_HH */

#ifndef    TIME_UTILS_HXX
# define   TIME_UTILS_HXX

# include "TimeUtils.hh"
# include <ctime>
# include <iomanip>
# include <sstream>

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

  inline
  std::string
  timeToString(const TimeStamp& t) noexcept {
    // See here:
    // https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string/34858704
    // And here:
    // https://stackoverflow.com/questions/32873659/how-can-i-get-current-time-of-day-in-milliseconds-in-c/32874098#32874098
    std::time_t tt = std::chrono::system_clock::to_time_t(t);
    std::tm tm = *std::gmtime(&tt); //GMT (UTC)

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()) -
              std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch());

    std::stringstream ss;
    ss << std::put_time( &tm, "UTC: %Y-%m-%d %H:%M:%S");
    ss << "." << ms.count() << "ms";

    return ss.str();
  }

  inline
  float
  diffInMs(const TimeStamp& start, const TimeStamp& end) noexcept {
    auto elapsed = end - start;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

    return ms.count();
  }

}

#endif    /* TIME_UTILS_HXX */

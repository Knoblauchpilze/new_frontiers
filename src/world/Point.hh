#ifndef    POINT_HH
# define   POINT_HH

namespace new_frontiers {

  /**
   * @brief - Very basic structure representing a point.
   */
  struct Point {
    float x;
    float y;
  };

  /**
   * @brief - Used to convert from a position and a
   *          target to a direction-based path.
   * @param s - the starting position of the segment.
   * @param t - the end position of the path segment.
   * @param xD - output argument holding the direction
   *             of the input path.
   * @param yD - output argument holding the direction
   *             of the input path.
   * @param d - output argument holding the length of
   *            the path.
   * @param threshold - the threshold to consider a path
   *                    to have `0` length.
   * @param - `true` if the path has not `0` length.
   */
  bool
  toDirection(const Point& s,
              const Point& t,
              float& xD,
              float& yD,
              float& d,
              float threshold = 0.0001f) noexcept;

}

# include "Point.hxx"

#endif    /* POINT_HH */

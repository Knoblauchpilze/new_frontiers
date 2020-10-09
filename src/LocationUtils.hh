#ifndef    LOCATION_UTILS_HH
# define   LOCATION_UTILS_HH

namespace new_frontiers {
  namespace distance {

    /**
     * @brief - Used to compute the distance between point
     *          `(x1, y1)` and `(x2, y2)`.
     * @param x1 - abscissa of the first point.
     * @param y1 - ordinate of the first point.
     * @param x2 - abscissa of the second point.
     * @param y2 - ordinate of the second point.
     * @return - the distance between the two points.
     */
    float
    d(float x1, float y1, float x2, float y2) noexcept;

    /**
     * @brief - Similar to `d` but returns the squared
     *          distance between two points.
     * @param x1 - abscissa of the first point.
     * @param y1 - ordinate of the first point.
     * @param x2 - abscissa of the second point.
     * @param y2 - ordinate of the second point.
     * @return - the squared distance between the two
     *           input points.
     */
    float
    d2(float x1, float y1, float x2, float y2) noexcept;

  }
}

# include "LocationUtils.hxx"

#endif    /* LOCATION_UTILS_HH */

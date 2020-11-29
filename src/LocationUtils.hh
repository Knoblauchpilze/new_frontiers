#ifndef    LOCATION_UTILS_HH
# define   LOCATION_UTILS_HH

# include "Point.hh"

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
     * @brief - Used to compute the distance between the
     *          two input points.
     * @param p1 - the first point.
     * @param p2 - the second point.
     * @return - the distance between the two points.
     */
    float
    d(const Point& p1, const Point& p2) noexcept;

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

    /**
     * @brief - Similar to `d` but returns the squared
     *          distance between two points.
     * @param p1 - the first point.
     * @param p2 - the second point.
     * @return - the distance between the two points.
     */
    float
    d2(const Point& p1, const Point& p2) noexcept;

    /**
     * @brief - Used to compute the angle associated to
     *          the input direction. The reference frame
     *          is set to be `0` when the input vector
     *          is aligned with the `x` axis.
     *          In case the length of the specified dir
     *          is close to `0` a `null` angle will be
     *          returned.
     *          The angle is in the range `[0; 2pi[`.
     * @param xDir - the abscissa of the direction.
     * @param yDir - the ordinate of the direction.
     * @param threshold - a threshold to consider the
     *                    direction to be `null`.
     * @return - the angle corresponding to the input
     *           direction.
     */
    float
    angleFromDirection(float xDir, float yDir, float threshold = 0.0001f) noexcept;

    /**
     * @brief - Very similar to the above method but takes
     *          two points in argument rather than a dir.
     * @param p1 - the first point of the segment defining
     *             the direction.
     * @param p2 - the second point of the segment defining
     *             the direction.
     * @param threshold - a threshold to consider points to
     *                    be at the same position.
     * @return - the corresponding angle between both points.
     */
    float
    angleFromDirection(const Point& p1,
                       const Point& p2,
                       float threshold = 0.0001f) noexcept;

    /**
     * @brief - Used to determine whether the point `p` lies
     *          in the cone defined by `o` with principal dir
     *          `xDir, yDir` and angle `theta`. The angle is
     *          assumed to be distributed equally on both
     *          sides of the principal direction.
     * @param - the tip of the cone.
     * @param xDir - the abscissa of the principal direction
     *               of the cone.
     * @param yDir - the ordinate of the principal direction
     *               of the cone.
     * @param theta - the span of the cone.
     * @param p - the point whose inclusion in the cone should
     *            be checked.
     * @return - `true` if the point lies inside the cone.
     */
    float
    isInCone(const Point& o,
             float xDir,
             float yDir,
             float theta,
             const Point& p) noexcept;
  }
}

# include "LocationUtils.hxx"

#endif    /* LOCATION_UTILS_HH */

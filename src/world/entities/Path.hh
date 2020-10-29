#ifndef    PATH_HH
# define   PATH_HH

# include "StepInfo.hh"

namespace new_frontiers {
  namespace path {

    /**
     * @brief - Convenience structure defining a path segment with
     *          its endpoints (a starting point and a target) and
     *          a direction.
     */
    struct Segment {
      float xS, yS;
      float xT, yT;

      float xD, yD;

      /**
       * @brief - Computes the length of this path in cells.
       * @return - the length of the path in cells.
       */
      float
      length() const noexcept;

      /**
       * @brief - Used to make sure that the starting and end
       *          postion of this segment are consistent with
       *          the dimensions of the world.
       * @param info - information about the world into which
       *               this segment is living.
       */
      void
      normalize(const StepInfo& info);
    };

    /**
     * @brief - Define a complete path, composed of one or
     *          more segments.
     */
    struct Path {
      float xH, yH;

      float xC, yC;
      int seg;

      std::vector<Segment> segments;

      std::vector<float> cPoints;

      /**
       * @brief - Used to determine whether this path is
       *          valid. We consider a path valid if it
       *          is composed of at least one segment.
       * @return - `true` if this path is valid.
       */
      bool
      valid() const noexcept;

      /**
       * @brief - Reset all information defined in this path
       *          and assume the entity is starting from the
       *          input position.
       * @param x - the new current abscissa of the entity
       *            following this path.
       * @param y - the new current ordinate of the entity
       *            following this path.
       */
      void
      clear(float x, float y);

      /**
       * @brief - Add the specified passage point in the list
       *          without changing it otherwise.
       * @param x - the abscissa of the passage point to add.
       * @param y - the ordinate of the passage point to add.
       */
      void
      addPassagePoint(float x, float y);

      /**
       * @brief - Add a new segment to this path with the
       *          specified information.
       * @param x - the abscissa of the starting location
       *            of the new segment.
       * @param y - the ordinate of the starting location
       *            of the new segment.
       * @param xD - the abscissa of the direction of the
       *             new segment.
       * @param yD - the ordinate of the direction of the
       *             new segment.
       * @param d - the length of the path segment.
       */
      void
      add(float x, float y, float xD, float yD, float d);

      /**
       * @brief - Other variant to register a new segment
       *          on this path.
       * @param xS - the abscissa of the starting location
       *             of the path segment.
       * @param yS - the ordinate of the starting location
       *             of the path segment.
       * @param xT - the abscissa of the end location of
       *             the path segment.
       * @param yT - the ordinate of the end location of
       *             the path segment.
       */
      void
      add(float xS, float yS, float xT, float yT);

      /**
       * @brief - Add a new path segment starting from the
       *          last position reached by the path and with
       *          the corresponding end position.
       *          In case no previous path segment exists
       *          the starting position of the path is used.
       * @param x - the desired abscissa for the end of the
       *            new path segment.
       * @param y - the desired ordinate for the end of the
       *            new path segment.
       */
      void
      add(float x, float y);

      /**
       * @brief - Determine whether the current position on
       *          the path means that we arrived or not. It
       *          is only precise if the user actually makes
       *          the path-follower advance on the path by
       *          calling `advance`.
       * @param threshold - define the threshold below which
       *                    the path-follower is considered
       *                    to have arrived.
       * @return - `true` in case the path-follower has not
       *           arrived yet.
       */
      bool
      enRoute(float threshold) const noexcept;

      /**
       * @brief - Used to advance on this path assuming the
       *          path follower is moving at `speed` along
       *          the path and that `elapsed` seconds have
       *          passed since the last actualization.
       * @param speed - the speef of the entity following
       *                the path.
       * @param elapsed - the duration elapsed since the
       *                  last actualization in seconds.
       * @param threshold - a threshold to consider that a
       *                    path segment is finished.
       */
      void
      advance(float speed, float elapsed, float threshold);
    };

    /**
     * @brief - Create a new path segment starting at the
     *          specified location and with the direction
     *          provided in input.
     *          The final position of the segment is given
     *          by measuring `d` units along the direction.
     * @param x - the starting abscissa of the segment.
     * @param y - the starting ordinate of the segment.
     * @param xD - the abscissa for the direction.
     * @param yD - the ordinate for the direction.
     * @param d - the length of this path segment.
     * @return - the created path segment.
     */
    Segment
    newSegment(float x, float y, float xD, float yD, float d) noexcept;

    /**
     * @brief - Create a new segment from a starting location
     *          and an end position.
     * @param xS - the starting abscissa for this segment.
     * @param yS - the starting ordinate for this segment.
     * @param xT - the end abscissa for this segment.
     * @param yT - the end ordinate for this segment.
     * @return - the created path segment.
     */
    Segment
    newSegment(float xS, float yS, float xT, float yT) noexcept;

    /**
     * @brief - Create a new path with the input position
     *          as default.
     * @param x - the starting abscissa of the path.
     * @param y - the starting ordinate of the path.
     * @return - the created path.
     */
    Path
    newPath(float x, float y) noexcept;

  }
}

# include "Path.hxx"

#endif    /* PATH_HH */

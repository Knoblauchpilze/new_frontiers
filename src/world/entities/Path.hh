#ifndef    PATH_HH
# define   PATH_HH

namespace new_frontiers {
  namespace path {

    /**
     * @brief - Convenience structure defining a path segment with
     *          its endpoints (a starting point and a target) and
     *          a direction.
     */
    struct Segment {
      float xO, yO;
      float xT, yT;

      float xD, yD;

      /**
       * @brief - Computes the length of this path in cells.
       * @return - the length of the path in cells.
       */
      float
      length() const noexcept;
    };

    /**
     * @brief - Define a complete path, composed of one or
     *          more segments.
     */
    struct Path {
      float xH, yH;
      Segment segments;

      std::vector<float> cPoints;

      /**
       * @brief - Reset the information defined in this path
       *          to include a single segment going from the
       *          starting position provided in input to the
       *          end location.
       * @param xS - the abscissa of the starting location.
       * @param yS - the ordinate of the starting location.
       * @param xT - the abscissa of the end location.
       * @param yT - the ordinate of the end location.
       */
      void
      reset(float xS, float yS, float xT, float yT);

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

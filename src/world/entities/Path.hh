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
      Segment segments;

      std::vector<float> cPoints;
    };

  }
}

# include "Path.hxx"

#endif    /* PATH_HH */

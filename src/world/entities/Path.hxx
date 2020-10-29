#ifndef    PATH_HXX
# define   PATH_HXX

# include "Path.hh"

namespace new_frontiers {
  namespace path {

    inline
    float
    Segment::length() const noexcept {
      return std::sqrt((xT - xO) * (xT - xO) + (yT - yO) * (yT - yO));
    }

    inline
    void
    Path::reset(float xS, float yS, float xT, float yT) {
      // Clear intermediate locations.
      cPoints.clear();

      // Register new way points as provided in input.
      cPoints.push_back(xS);
      cPoints.push_back(yS);

      cPoints.push_back(xT);
      cPoints.push_back(yT);
    }

    inline
    void
    Path::add(float x, float y, float xD, float yD, float d) {
      segments = newSegment(x, y, xD, yD, d);
    }

    inline
    void
    Path::add(float xS, float yS, float xT, float yT) {
      // Convert the path to a format allowing to use
      // the other variant.
      float xD = xT - xS;
      float yD = yT - yS;
      float d = std::sqrt(xD * xD + yD * yD);

      add(xS, yS, xD, yD, d);
    }

    inline
    void
    Path::add(float x, float y) {
      add(xH, yH, x, y);
    }

    inline
    Segment
    newSegment(float x, float y, float xD, float yD, float d) noexcept {
      Segment s;

      s.xO = x;
      s.yO = y;
      s.xD = xD;
      s.yD = yD;

      s.xT = s.xO + d * s.xD;
      s.yT = s.yO + d * s.yD;

      return s;
    }

    inline
    Path
    newPath(float x, float y) noexcept {
      Path p;

      p.xH = x;
      p.yH = y;
      p.segments = newSegment(x, y, 1.0f, 0.0f, 0.0f);

      return p;
    }

  }
}

#endif    /* PATH_HXX */

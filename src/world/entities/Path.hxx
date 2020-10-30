#ifndef    PATH_HXX
# define   PATH_HXX

# include "Path.hh"
# include "LocationUtils.hh"

namespace new_frontiers {
  namespace path {

    inline
    float
    Segment::length() const noexcept {
      return std::sqrt((xT - xS) * (xT - xS) + (yT - yS) * (yT - yS));
    }

    inline
    void
    Segment::normalize(const StepInfo& info) {
      // Normalize starting location and also the
      // end location.
      info.clampCoord(xS, yS);
      info.clampCoord(xT, yT);

      // Update direction for this segment.
      xD = xT - xS;
      yD = yT - yS;

      float d = length();
      if (d > 0.0001f) {
        xD /= d;
        yD /= d;
      }
    }

    inline
    bool
    Path::valid() const noexcept {
      return !segments.empty();
    }

    inline
    void
    Path::clear(float x, float y) {
      // Assign home and current position.
      xH = x;
      yH = y;

      xC = x;
      yC = y;

      // Reset segments.
      seg = -1;
      segments.clear();

      // Reset temporary passage points.
      cPoints.clear();
      addPassagePoint(x, y);
    }

    inline
    void
    Path::addPassagePoint(float x, float y) {
      cPoints.push_back(x);
      cPoints.push_back(y);
    }

    inline
    void
    Path::add(float x, float y, float xD, float yD, float d) {
      Segment s = newSegment(x, y, xD, yD, d);
      segments.push_back(s);
      addPassagePoint(s.xT, s.yT);

      // Make the entity on the first segment.
      if (seg < 0) {
        seg = 0;
      }
    }

    inline
    void
    Path::add(float xS, float yS, float xT, float yT) {
      Segment s = newSegment(xS, yS, xT, yT);
      segments.push_back(s);
      addPassagePoint(s.xT, s.yT);

      // Make the entity on the first segment.
      if (seg < 0) {
        seg = 0;
      }
    }

    inline
    void
    Path::add(float x, float y) {
      if (seg < 0) {
        add(xH, yH, x, y);
      }
      else {
        const Segment s = segments.back();
        add(s.xT, s.yT, x, y);
      }
    }

    inline
    bool
    Path::enRoute(float threshold) const noexcept {
      // In case the identifier does not describe a
      // valid path segment, assume we did arrive.
      // Similarly if no segments are registered we
      // consider that we already arrived.
      int ss = static_cast<int>(segments.size());
      if (seg < 0 || seg >= ss) {
        return false;
      }

      return (seg < ss - 1) || distance::d(segments[seg].xT, segments[seg].yT, xC, yC) > threshold;
    }

    inline
    void
    Path::advance(float speed, float elapsed, float threshold) {
      // In case we already arrived, do nothing.
      if (!enRoute(threshold)) {
        return;
      }

      float traveled = speed * elapsed;
      int ss = static_cast<int>(segments.size());

      // Attempt to complete the current path
      // segment until we either reach the end
      // of the path or don't travel enough to
      // do so.
      float dToEofS = distance::d(xC, yC, segments[seg].xT, segments[seg].yT);

      while (traveled > dToEofS && seg < ss) {
        // Move along the path segment: as we
        // traveled far enough to complete the
        // path segment we can directly go to
        // the target point of the segment.
        xC = segments[seg].xT;
        yC = segments[seg].yT;

        // Finish this path segment and move to
        // the next one.
        traveled -= dToEofS;
        ++seg;

        // In order to prepare for the advance of
        // the segment, we will move to the start
        // of the next segment (so the one pointed
        // by `seg` now).
        // This should usually correspond to the
        // end of the previous segment but as it
        // is not guaranteed, we will ensure it.
        if (seg < ss) {
          xC = segments[seg].xS;
          yC = segments[seg].yS;
        }

        // Initialize the distance to the end
        // of the path segment.
        dToEofS = distance::d(xC, yC, segments[seg].xT, segments[seg].yT);
      }

      // See whether we reached the end of the
      // path: if this is the case, the previous
      // algo ensured we were already at the end
      // of the path so we don't have nothing
      // else to do.
      if (seg == ss) {
        return;
      }

      // Advance on the path of the amount left.
      xC += traveled * segments[seg].xD;
      yC += traveled * segments[seg].yD;
    }

    inline
    Segment
    newSegment(float x, float y, float xD, float yD, float d) noexcept {
      Segment s;

      s.xS = x;
      s.yS = y;
      s.xD = xD;
      s.yD = yD;

      s.xT = s.xS + d * s.xD;
      s.yT = s.yS + d * s.yD;

      return s;
    }

    inline
    Segment
    newSegment(float xS, float yS, float xT, float yT) noexcept {
      Segment s;

      s.xS = xS;
      s.yS = yS;
      s.xT = xT;
      s.yT = yT;

      s.xD = s.xT - s.xS;
      s.yD = s.yT - s.yS;

      float d = s.length();
      if (d > 0.0001f) {
        s.xD /= d;
        s.yD /= d;
      }

      return s;
    }

    inline
    Path
    newPath(float x, float y) noexcept {
      Path p;

      p.xH = x;
      p.yH = y;

      p.xC = x;
      p.yC = y;

      p.seg = -1;
      p.addPassagePoint(x, y);

      return p;
    }

  }
}

#endif    /* PATH_HXX */

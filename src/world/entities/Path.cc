
# include "Path.hh"

namespace new_frontiers {
  namespace path {

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

    void
    Path::generatePathTo(StepInfo& /*info*/, float x, float y) {
      // TODO: Implement this.
      add(x, y);
    }

  }
}

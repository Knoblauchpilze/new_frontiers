
# include "Path.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "AStar.hh"

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
      float dToEofS = utils::d(cur, segments[seg].end);

      while (traveled > dToEofS && seg < ss) {
        // Move along the path segment: as we
        // traveled far enough to complete the
        // path segment we can directly go to
        // the target point of the segment.
        cur.x() = segments[seg].end.x();
        cur.y() = segments[seg].end.y();

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
          cur.x() = segments[seg].start.x();
          cur.y() = segments[seg].start.y();
        }

        // Initialize the distance to the end
        // of the path segment.
        dToEofS = utils::d(cur, segments[seg].end);
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
      cur.x() += traveled * segments[seg].xD;
      cur.y() += traveled * segments[seg].yD;
    }

    bool
    Path::generatePathTo(StepInfo& info,
                         const utils::Point2f& p,
                         bool ignoreTargetObstruction,
                         float maxDistanceFromStart,
                         bool allowLog)
    {
      // Convert to path semantic: the starting point
      // of the path segment will be the end of the
      // registered list of intermediate points. It
      // can correspond to the home position in case
      // no segments are defined.
      utils::Point2f s = home;
      if (seg >= 0) {
        s = segments[segments.size() - 1].end;
      }

      float xDir, yDir, d;
      utils::toDirection(s, p, xDir, yDir, d);

      // Detect trivial case where the target is a
      // solid block and we're not supposed to be
      // ignoring it: in this case it does not make
      // sense to try to generate a path.
      if (info.frustum->obstructed(p) && !ignoreTargetObstruction) {
        return false;
      }

      // First, try to find a straight path to the
      // target: if this is possible it's cool.
      utils::Point2f obsP;
      std::vector<utils::Point2f> iPoints;

      bool obs = info.frustum->obstructed(s, xDir, yDir, d, iPoints, &obsP);
      bool obsWithinTarget = obs && (std::abs(obsP.x() - p.x()) < 1.0f && std::abs(obsP.y() - p.y()) < 1.0f);

      if (!obs || (obsWithinTarget && ignoreTargetObstruction)) {
        // Either there is no obstruction or there is one
        // but we ignore it. In case we could generate at
        // least one intermediate point between the start
        // and the end of the path, we need to register
        // it.
        if (!iPoints.empty()) {
          for (unsigned id = 0u ; id < iPoints.size() - 1 ; ++id) {
            cPoints.push_back(iPoints[id]);
          }
        }

        add(p);

        return true;
      }

      // Note that at this point we know:
      //  - that the target is not obstructed or we
      //    ignore it.
      //  - there is no straight path to the target.
      // So with this information in mind, we will
      // try to run a A* algorithm. We thought about
      // relying on a technique called the `obstacle
      // tracing` algorithm but it appears that it's
      // not really easy to implement and we prefer
      // to choose safety. It is not exactly what we
      // desired (the A*) as it means that each mob
      // has indeed infinite vision for now but
      // that's it.
      AStar alg(s, p, info.frustum);
      std::vector<utils::Point2f> steps;

      if (!alg.findPath(steps, maxDistanceFromStart, allowLog)) {
        return false;
      }

      // A path was found, register it.
      for (unsigned id = 0u ; id < steps.size() ; ++id) {
        add(steps[id]);
      }

      return true;
    }

  }
}


# include "Path.hh"
# include "StepInfo.hh"
# include "Locator.hh"

# include <iostream>

namespace {

  enum class Axis {
    X,
    Y
  };

  // void
  // nextCell(float x, float y, float xDir, float yDir, float& xN, float& yN) {
  //   // We need to compute the first point on the boundary
  //   // of the cell which will be intersected by the path
  //   // defined by `[(x, y); (xDir, yDir)]`.
  //   // We will handle the trivial case of no direction in
  //   // a first time.
  //   float xi = std::floor(x), yi = std::floor(y);

  //   float l = std::sqrt(xDir * xDir + yDir * yDir);
  //   if (l < 0.0001f) {
  //     xN = x;
  //     yN = y;

  //     return;
  //   }

  //   // Compute how many times the `xDir` and `yDir` can
  //   // fit respectively in the `x` and `y` direction.
  //   float dx = (xDir < 0.0f ? x - xi : std::ceil(x) - x);
  //   float dy = (yDir < 0.0f ? y - yi : std::ceil(y) - y);

  //   float cx = dx / xDir, cy = dy / yDir;

  //   // Whichever of `cx` or `cy` is smaller means that
  //   // it will be reached first.
  //   if (cx < cy) {
  //     xN = xi + std::copysign(1.0f, xDir);
  //     yN = yi;
  //   }
  //   else if (cy < cx) {
  //     xN = xi;
  //     yN = yi + std::copysign(1.0f, yDir);
  //   }
  //   else {
  //     xN = xi + std::copysign(1.0f, xDir);
  //     yN = yi + std::copysign(1.0f, yDir);
  //   }
  // }

}

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
      float dToEofS = distance::d(cur, segments[seg].end);

      while (traveled > dToEofS && seg < ss) {
        // Move along the path segment: as we
        // traveled far enough to complete the
        // path segment we can directly go to
        // the target point of the segment.
        cur.x = segments[seg].end.x;
        cur.y = segments[seg].end.y;

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
          cur.x = segments[seg].start.x;
          cur.y = segments[seg].start.y;
        }

        // Initialize the distance to the end
        // of the path segment.
        dToEofS = distance::d(cur, segments[seg].end);
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
      cur.x += traveled * segments[seg].xD;
      cur.y += traveled * segments[seg].yD;
    }

    void
    Path::generatePathTo(StepInfo& info, const Point& p, bool ignoreTargetObstruction) {
      // Convert to path semantic: the starting point
      // of the path segment will be the end of the
      // registered list of intermediate points. It
      // can correspond to the home position in case
      // no segments are defined.
      Point s = home;
      if (seg >= 0) {
        s = segments[segments.size() - 1].end;
      }

      float xDir, yDir, d;
      Point obsP;
      info.toDirection(s, p, xDir, yDir, d);

      // TODO: What happens when we don't want to
      // ignore obstruction of the target but the
      // target is actually a solid block ?
      // TODO: Case where `xDir` or `yDir` is `0`.
      // TODO: Case where we reach a position that
      // was already explored. This is what prevents
      // the code from working right now.

      // Process the path: we want to keep finding
      // intermediate positions until we can find
      // an unobstructed path to the target. This
      // could very well be right away in case the
      // path is directly visible.
      std::vector<Point> points;
      std::vector<Point> steps;

      bool pathNotClear = true;
      int count = 0;
      while (pathNotClear && count < 20) {
        // Register the starting position of this
        // path segment: this is the main way to
        // be sure that each segment is correctly
        // saved. The path struct has a mechanism
        // to make sure that we don't add twice
        // the home coordinates: for the rest we
        // assume that we won't try to register
        // the same position more than once.
        steps.push_back(s);
        points.push_back(s);

        std::vector<Point> iPoints;
        bool obs = info.frustum->obstructed(s, xDir, yDir, d, iPoints, &obsP);

        // If the path is obstructed, we need to
        // march on the border of the obstacle
        // until we walk around it. Note that we
        // only consider an obstruction if the
        // input argument says so.
        bool obsWithinTarget = (std::abs(obsP.x - p.x) < 1.0f && std::abs(obsP.y - p.y) < 1.0f);

        // We want to process the potential
        // intersection only if it is meaningful.
        pathNotClear = (
          obs // In case there is an obstruction
          &&  // and
          (
            !obsWithinTarget // the obstruction does not
                             // lie within the target
            || // or (in case the obstruction is within
               // the target)
            !ignoreTargetObstruction // we aren't ignoring
                                      // obstruction of the
                                      // cell.
          )
        );

        if (!pathNotClear) {
          points.insert(points.end(), iPoints.cbegin(), iPoints.cend());
          std::cout << "[PATH][" << segments.size() << "] start: " << s.x << "x" << s.y
                    << " to " << p.x << "x" << p.y
                    << " is unobstructed, registering "
                    << s.x << "x" << s.y
                    << std::endl;
        }
        else {
          // Determine in which way we want to
          // explore for unobstructed regions
          // in the neighborhood of the path.
          float aXDir = std::abs(xDir);
          float aYDir = std::abs(yDir);

          // Axis dPerp = (aXDir > aYDir ? Axis::X : Axis::Y);
          Axis dPara = (aXDir > aYDir ? Axis::Y : Axis::X);

          float signX = std::copysign(1.0f, xDir);
          float signY = std::copysign(1.0f, yDir);
          // float sPerp = (dPerp == Axis::X ? signX : signY);
          float sPara = (dPara == Axis::X ? signX : signY);

          // Register the intermediate steps that were
          // deemed valid by the exploration: this will
          // get us as far as possible on this segment.
          // To determine the last valid position we
          // will get the second-to-last position in
          // the `iPoints` vector: this should be the
          // last valid position along the path.
          // We can then start from there the search
          // for a path around the obstacle.
          Point pStop = s;

          if (iPoints.size() > 2u) {
            // Note that as we already pushed the
            // steps related to the start position
            // we will start at the second point
            // reached by the obstruction detection
            // process. This mean that we might not
            // register anything.
            for (unsigned id = 1u ; id < iPoints.size() - 1u ; ++id) {
              points.push_back(iPoints[id]);
            }

            pStop = iPoints[iPoints.size() - 2u];
            steps.push_back(pStop);
          }

          float xSave = s.x, ySave = s.y;

          // Update the starting position and direction
          // by placing the next intermediate position
          // one cell on the direction of `dPara`.
          s = pStop;

          switch (dPara) {
            case Axis::X:
              s.x += sPara;
              break;
            case Axis::Y:
            default:
              s.y += sPara;
              break;
          }

          std::cout << "[PATH][" << segments.size() << "] start: " << xSave << "x" << ySave
                    << " to " << p.x << "x" << p.y
                    << " is obstructed at "
                    << obsP.x << "x" << obsP.y
                    << ", (d: " << xDir << "x" << yDir << ")"
                    << " stop: " << pStop.x << "x" << pStop.y
                    << ", registering: " << xSave << "x" << ySave
                    << " and " << pStop.x << "x" << pStop.y
                    << ", starting from " << s.x << "x" << s.y
                    << std::endl;

          info.toDirection(s, p, xDir, yDir, d);
        }

        ++count;
      }

      // Once a valid path is built, register it
      // as the actual path.
      for (unsigned id = 0u ; id < steps.size() ; ++id) {
        std::cout << "[PATH][END][" << segments.size() << "] registering "
                  << steps[id].x << "x" << steps[id].y
                  << std::endl;
        add(steps[id]);
      }
      add(p);

      cPoints.swap(points);
    }

  }
}

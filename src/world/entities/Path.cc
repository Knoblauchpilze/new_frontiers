
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
    Path::generatePathTo(StepInfo& info, float x, float y, bool ignoreTargetObstruction) {
      // Convert to path semantic. The starting point
      // of the path segment will be the end of the
      // registered list.
      float xS = xH, yS = yH;
      if (seg >= 0) {
        xS = segments[segments.size() - 1].xT;
        yS = segments[segments.size() - 1].yT;
      }

      float xDir, yDir, d, xObs, yObs;
      info.toDirection(xS, yS, x, y, xDir, yDir, d);

      // TODO: What happens when we don't want to
      // ignore obstruction of the target but the
      // target is actually a solid block ?

      // Process the path: we want to keep finding
      // intermediate positions until we can find
      // an unobstructed path to the target. This
      // could very well be right away in case the
      // path is directly visible.
      std::vector<float> points;

      bool obs = true;
      int count = 0;
      while (obs && count < 20) {
        // TODO: Obstruction should include the
        // fractional part of the cell ?
        obs = info.frustum->obstructed(xS, yS, xDir, yDir, d, points, &xObs, &yObs);

        // Register the starting position of this
        // path segment.
        add(xS, yS);

        // If the path is obstructed, we need to
        // march on the border of the obstacle
        // until we walk around it. Note that we
        // only consider an obstruction if the
        // input argument says so.
        bool obsWithinTarget = (
          ignoreTargetObstruction &&
          std::abs(xObs - x) < 1.0f &&
          std::abs(yObs - y) < 1.0f
        );

        if (obs && !obsWithinTarget) {
          // Determine in which way we want to
          // explore for unobstructed regions
          // in the neighborhood of the path.
          float aXDir = std::abs(xDir);
          float aYDir = std::abs(yDir);

          Axis dPerp = (aXDir > aYDir ? Axis::X : Axis::Y);
          Axis dPara = (aXDir > aYDir ? Axis::Y : Axis::X);

          float signX = std::copysign(1.0f, xDir);
          float signY = std::copysign(1.0f, yDir);
          float sPerp = (dPerp == Axis::X ? signX : signY);
          float sPara = (dPara == Axis::X ? signX : signY);

          // Compute the maximum position we can
          // reach: we must move one cell behind
          // the reported obstruction.
          float xStop = xObs + 0.5f;
          float yStop = yObs + 0.5f;

          switch (dPerp) {
            case Axis::X:
              xStop -= sPerp;
              break;
            case Axis::Y:
            default:
              yStop -= sPerp;
              break;
          }

          // Register the stopping position of the
          // path as an intermediate position.
          add(xStop, yStop);

          float xSave = xS, ySave = yS;

          // Update the starting position and direction
          // by placing the next intermediate position
          // one cell on the direction of `dPara`.
          xS = xStop;
          yS = yStop;

          switch (dPara) {
            case Axis::X:
              xS += sPara;
              break;
            case Axis::Y:
            default:
              yS += sPara;
              break;
          }

          std::cout << "[PATH][" << segments.size() << "] start: " << xSave << "x" << ySave
                    << " to " << x << "x" << y
                    << " is obstructed at "
                    << xObs << "x" << yObs
                    << ", (d: " << xDir << "x" << yDir << ")"
                    << " stop: " << xStop << "x" << yStop
                    << ", registering: " << xSave << "x" << ySave
                    << ", starting from " << xS << "x" << yS
                    << std::endl;

          info.toDirection(xS, yS, x, y, xDir, yDir, d);
        }

        ++count;
      }

      // TODO: Implement this.
      add(x, y);
    }

  }
}


# include "Path.hh"
# include "StepInfo.hh"
# include "Locator.hh"

# include <iostream>

namespace {

  enum class Axis {
    X,
    Y
  };

  enum class Direction {
    None,
    PositiveX,
    NegativeX,
    PositiveY,
    NegativeY
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

  /**
   * @brief - Compute the offset to apply to the position we're
   *          considering the input direction and generate the
   *          next position to consider.
   * @param p - the current position to which the offset will
   *            be applied.
   * @param xDir - the abscissa of the direction we're using.
   * @param yDir - the ordinate of the direction we're using.
   * @param dir - the current direction of travel. Will be used
   *              to update the followed direction.
   * @param loc - a locator to help take decisions and provide
   *              a next unobstructed position.
   * @return - a set of two coordinates representing the next
   *           position to consider.
   */
  new_frontiers::Point
  computeNextPosition(new_frontiers::Point p,
                      float xDir,
                      float yDir,
                      Direction& dir,
                      const new_frontiers::Locator& loc)
  {
    // Compute the parallel direction (i.e. the direction
    // with the least amount of momentum).
    float aXDir = std::abs(xDir);
    float aYDir = std::abs(yDir);

    Axis dPara = (aXDir > aYDir ? Axis::Y : Axis::X);

    // The direction of the offset will be the same as
    // it was before (if any value is assigned) unless
    // the direction has switched to the perpendicular
    // direction of the previous one. We also have to
    // take into account cases where the direction can
    // be resumed to something closer to the target if
    // the path is not unobstructed anymore.
    // Note that by default we use a positive direction
    // to explore.

    // So first, determine the direction as it would
    // be in case we were to start anew.
    Axis d = dPara;
    float sPara = 1.0f;

    switch (dir) {
      case Direction::PositiveX:
        d = Axis::X;
        sPara = 1.0f;
        break;
      case Direction::PositiveY:
        d = Axis::Y;
        sPara = 1.0f;
        break;
      case Direction::NegativeX:
        d = Axis::X;
        sPara = -1.0f;
        break;
      case Direction::NegativeY:
        d = Axis::Y;
        sPara = -1.0f;
        break;
      case Direction::None:
      default:
        // Only `dir` is left to initialize. Choose
        // the positive direction by default.
        if (d == Axis::X) {
          sPara = std::copysign(1.0f, xDir);
          dir = (sPara > 0.0f ? Direction::PositiveX : Direction::NegativeX);
        }
        else {
          sPara = std::copysign(1.0f, yDir);
          dir = (sPara > 0.0f ? Direction::PositiveY : Direction::NegativeY);
        }
        break;
    }

    std::cout << "[PATH] point: " << p.x << "x" << p.y
              << " d: " << xDir << "x" << yDir
              << ", dPara: " << (d == Axis::X ? "x" : "y")
              << ", sign: " << sPara
              << ", computed dPara: " << (dPara == Axis::X ? "x" : "y")
              << ", dir is " << (dir == Direction::None ? "none" : (dir == Direction::PositiveX ? "pos x" : (dir == Direction::PositiveY ? "pos y" : (dir == Direction::NegativeX ? "neg x" : "neg y"))))
              << std::endl;

    // Make sure that we continue in the same
    // direction as we were except if the new
    // direction is perpendicular.
    if ((dir == Direction::PositiveX || dir == Direction::NegativeX) && dPara == Axis::Y) {
      sPara = std::copysign(1.0f, yDir);
      dir = (sPara > 0.0f ? Direction::PositiveY : Direction::NegativeY);
    }
    else if ((dir == Direction::PositiveY || dir == Direction::NegativeY) && dPara == Axis::X) {
      sPara = std::copysign(1.0f, xDir);
      dir = (sPara > 0.0f ? Direction::PositiveX : Direction::NegativeX);
    }

    new_frontiers::Point op = p;
    switch (d) {
      case Axis::X:
        op.x += sPara;
        break;
      case Axis::Y:
      default:
        op.y += sPara;
        break;
    }

    // Make sure that the chosen cell is unobstructed:
    // if it is the case we will have to rotate so we
    // continue to trace the obstacle.
    if (!loc.obstructed(op)) {
      return op;
    }

    // Rotate in order to follow the obstacle's border.
    Direction rd = dir;
    switch (dir) {
      case Direction::PositiveX:
        rd = Direction::NegativeY;
        d = Axis::Y;
        sPara = -1.0f;
        break;
      case Direction::PositiveY:
        rd = Direction::PositiveX;
        d = Axis::X;
        sPara = 1.0f;
        break;
      case Direction::NegativeX:
        rd = Direction::PositiveY;
        d = Axis::Y;
        sPara = 1.0f;
        break;
      case Direction::NegativeY:
        rd = Direction::NegativeX;
        d = Axis::X;
        sPara = -1.0f;
        break;
      case Direction::None:
      default:
        // Should never happen.
        break;
    }

    new_frontiers::Point ops = op;

    op = p;
    switch (d) {
      case Axis::X:
        op.x += sPara;
        break;
      case Axis::Y:
      default:
        op.y += sPara;
        break;
    }

    std::cout << "[PATH] point: " << p.x << "x" << p.y
              << " d: " << xDir << "x" << yDir
              << ", led to " << ops.x << "x" << ops.y
              << " but was obstructed so chose "
              << op.x << "x" << op.y
              << " (dir: " << (rd == Direction::None ? "none" : (rd == Direction::PositiveX ? "pos x" : (rd == Direction::PositiveY ? "pos y" : (rd == Direction::NegativeX ? "neg x" : "neg y")))) << ")"
              << std::endl;

    return op;
  }

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
      Direction dir = Direction::None;
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

        std::cout << "-----" << std::endl;
        std::cout << "Step " << count << std::endl;

        std::vector<Point> iPoints;
        bool obs = info.frustum->obstructed(s, xDir, yDir, d, iPoints, &obsP);

        // If the path is obstructed, we need to
        // march on the border of the obstacle
        // until we walk around it. Note that we
        // only consider an obstruction if the
        // input argument says so.
        bool obsWithinTarget = obs && (std::abs(obsP.x - p.x) < 1.0f && std::abs(obsP.y - p.y) < 1.0f);

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

          // Determine in which way we want to explore
          // for unobstructed regions in the neighborhood
          // of the path.
          s = computeNextPosition(pStop, xDir, yDir, dir, *info.frustum);

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
        std::cout << "-----" << std::endl;
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

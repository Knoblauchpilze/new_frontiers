
# include "Entity.hh"
# include "WorldElementLocator.hh"


namespace new_frontiers {

  bool
  Entity::step(StepInfo& info) {
    // Choose a random speed if none is assigned yet.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.1f, 3.0f);
    }

    // If we did not arrive yet at our destination we
    // need to move towards it.
    if (m_hasPath) {
      float dToT = info.frustum->d(m_path.xT, m_path.yT, m_tile.x, m_tile.y);
      m_hasPath = (dToT > m_rArrival);
    }

    // In case we don't have a path, select one.
    if (!m_hasPath) {
      choosePath(info);
    }

    // Move along the path.
    if (m_hasPath) {
      m_path.animate(info.moment, m_tile.x, m_tile.y);
    }

    return true;
  }

  void
  Entity::choosePath(const StepInfo& info) {
    // We always start from the current position.
    m_path.xO = m_tile.x;
    m_path.yO = m_tile.y;

    // Pick a random location within the radius of the
    // motion for this entity. We will use the locator
    // to determine if any element is obstructing the
    // view.
    // We want at least a path of `m_speed` long so as
    // to have a traversal duration of at least 1ms.
    // we also take a bit of margin for good measure.
    float r = info.rng.rndFloat(m_speed, m_pathLength);
    float theta = info.rng.rndAngle();

    float xDir = std::cos(theta);
    float yDir = std::sin(theta);

    float xt = m_path.xO + r * xDir;
    float yt = m_path.yO + r * yDir;

    log(
      "Attempt o(" + std::to_string(m_path.xO) + "x" + std::to_string(m_path.yO) +
      " to " + std::to_string(xt) + "x" + std::to_string(yt),
      utils::Level::Debug
    );

    while (info.frustum->obstructed(m_path.xO, m_path.yO, xDir, yDir, r)) {
      r = info.rng.rndFloat(m_speed, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      xt = m_path.xO + r * xDir;
      yt = m_path.yO + r * yDir;

      log(
        "Attempt o(" + std::to_string(m_path.xO) + "x" + std::to_string(m_path.yO) +
        " to " + std::to_string(xt) + "x" + std::to_string(yt),
        utils::Level::Debug
      );
    }

    m_path.xT = m_path.xO + r * xDir;
    m_path.yT = m_path.yO + r * yDir;

    info.clampCoord(m_path.xT, m_path.yT);

    // We start right now: the end time depends on the
    // speed of the entity and the length of the path.
    // Note: we might run into some issues in case the
    // start and end duration are the same: typically
    // the entity will 'jump' from the starting point
    // to the end point. It should not be an issue as
    // the endpoints will be close anyways but still.
    // Remember that.
    m_path.start = info.moment;
    m_path.end = m_path.start + toMilliseconds(static_cast<int>(1000.0f * m_path.length() / m_speed));

    // We know have a path.
    m_hasPath = true;
  }

}

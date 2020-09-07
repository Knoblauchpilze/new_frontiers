
# include "Entity.hh"
# include "Locator.hh"


namespace new_frontiers {

  bool
  Entity::step(StepInfo& info) {
    // Choose a random speed if none is assigned yet.
    // TODO: Maybe this could be moved in the `preSpawn`
    // function of the spawner.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.1f, 3.0f);
    }

    // If we did not arrive yet at our destination we
    // need to move towards it.
    if (m_hasPath) {
      float dToT = info.frustum->d(m_path.xT, m_path.yT, m_tile.x, m_tile.y);
      m_hasPath = (dToT > m_rArrival);

# ifdef DEBUG
      if (!m_hasPath) {
        log(
          "Reached o(" + std::to_string(m_tile.x) + "x" + std::to_string(m_tile.y) +
          "), " + std::to_string(dToT) + " from t(" +
          std::to_string(m_path.xT) + "x" + std::to_string(m_path.yT) + ") (r: " +
          std::to_string(m_rArrival) + ")",
          utils::Level::Debug
        );
      }
# endif
    }

    // In case we don't have a path, select one.
    if (!m_hasPath) {
      choosePath(info);
    }

    // Move along the path.
    bool moved = false;
// # define NO_MOTION
# ifdef NO_MOTION
    if (m_hasPath && false) {
# else
    if (m_hasPath) {
# endif
      m_path.animate(info.moment, m_tile.x, m_tile.y);
      moved = true;
    }

    return moved;
  }

  void
  Entity::choosePath(const StepInfo& info) {
    // Make the entity take an action: this will
    // provide a coordinate to go to.
    float x = 0.0f, y = 0.0f;
    takeAction(info, x, y);

    // Build the path information from the location
    // that was picked: basically this means clamp
    // any invalid information.
    float xDir = x - m_tile.x;
    float yDir = y - m_tile.y;

    float d = 1.0f;
    normalizePath(info, xDir, yDir, d);

    // TODO: Weird stuff with the path debug display.
    m_path.xO = m_tile.x;
    m_path.yO = m_tile.y;
    m_path.xT = m_path.xO + d * xDir;
    m_path.yT = m_path.yO + d * yDir;

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


# include "Worker.hh"
# include "Locator.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  Worker::Worker(const EntityTile& tile):
    Mob(tile)
  {
    setService("worker");
  }

  bool
  Worker::wander(StepInfo& info, float& x, float& y) {
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

    // Clamp these coordinates and update the direction
    // based on that.
    normalizePath(info, xDir, yDir, r);

# ifdef DEBUG
    float xt = m_tile.x + r * xDir;
    float yt = m_tile.y + r * yDir;

    log(
      "Attempt o(" + std::to_string(m_tile.x) + "x" + std::to_string(m_tile.y) +
      ") to t(" + std::to_string(xt) + "x" + std::to_string(yt) + ")",
      utils::Level::Debug
    );
# endif

    while (info.frustum->obstructed(m_tile.x, m_tile.y, xDir, yDir, r, m_cPoints)) {
# ifdef DEBUG
      log("Failed", utils::Level::Error);
# endif

      r = info.rng.rndFloat(m_speed, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      normalizePath(info, xDir, yDir, r);

# ifdef DEBUG
      xt = m_tile.x + r * xDir;
      yt = m_tile.y + r * yDir;
      log(
        "Attempt o(" + std::to_string(m_tile.x) + "x" + std::to_string(m_tile.y) +
        ") to t(" + std::to_string(xt) + "x" + std::to_string(yt) + ")",
        utils::Level::Debug
      );
# endif
    }

    // Save the picked location.
    x = m_tile.x + r * xDir;
    y = m_tile.y + r * yDir;

    return true;
  }

}

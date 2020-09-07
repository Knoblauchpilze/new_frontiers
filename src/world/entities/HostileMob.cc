
# include "HostileMob.hh"
# include "Locator.hh"

namespace new_frontiers {

  bool
  HostileMob::step(StepInfo& info) {
    // Perform the behavior of the entity.
    bool changed = behave(info);

    // Emit a new VFX if needed: that is if
    // we emitted it too long ago or if the
    // behavior just changed.
    if (changed || m_last + m_vfxDelay <= info.moment) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.vSpawned.push_back(spawnPheromon(pt));

      m_last = info.moment;
    }

    // TODO: Should we keep that or integrate it
    // in a `postStep` method provided by the base
    // class or something ?

    // Execute the base function and use it
    // as a return value.
    return Entity::step(info);
  }

  void
  HostileMob::takeAction(const StepInfo& info, float& x, float& y) {
    // TODO: We should refine this method: it should select
    // a path based on the current behavior and on the
    // pheromons visible.

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
    float xt = m_path.xO + r * xDir;
    float yt = m_path.yO + r * yDir;

    log(
      "Attempt o(" + std::to_string(m_path.xO) + "x" + std::to_string(m_path.yO) +
      ") to t(" + std::to_string(xt) + "x" + std::to_string(yt) + ")",
      utils::Level::Debug
    );
# endif

    while (info.frustum->obstructed(m_path.xO, m_path.yO, xDir, yDir, r, m_cPoints)) {
# ifdef DEBUG
      log("Failed", utils::Level::Error);
# endif

      r = info.rng.rndFloat(m_speed, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      normalizePath(info, xDir, yDir, r);

# ifdef DEBUG
      xt = m_path.xO + r * xDir;
      yt = m_path.yO + r * yDir;
      log(
        "Attempt o(" + std::to_string(m_path.xO) + "x" + std::to_string(m_path.yO) +
        ") to t(" + std::to_string(xt) + "x" + std::to_string(yt) + ")",
        utils::Level::Debug
      );
# endif
    }

    // Save the picked location.
    x = m_tile.x + r * xDir;
    y = m_tile.y + r * yDir;
  }

  void
  HostileMob::chase(StepInfo& /*info*/) {
    // TODO: Implement chase behavior.
  }

  void
  HostileMob::fight(StepInfo& /*info*/) {
    // TODO: Implement fight behavior.
  }

  void
  HostileMob::collect(StepInfo& /*info*/) {
    // TODO: Implement collect behavior.
  }

  void
  HostileMob::getBack(StepInfo& /*info*/) {
    // TODO: Implement return behavior.
  }

  void
  HostileMob::wander(StepInfo& /*info*/) {
    // TODO: Implement wander behavior.
  }

}

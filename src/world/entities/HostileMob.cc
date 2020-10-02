
# include "HostileMob.hh"
# include "Locator.hh"

namespace new_frontiers {

  HostileMob::HostileMob(const EntityTile& tile):
    Entity(tile, 0.5f),

    m_vfxDelay(toMilliseconds(8000)),
    m_last(now() - m_vfxDelay),

    m_passed(),

    m_behavior(Behavior::Wander)
  {}

  void
  HostileMob::takeAction(StepInfo& info, float& x, float& y) {
    // TODO: We should refine this method: it should select
    // a path based on the current behavior and on the
    // pheromons visible.

    // First, we need to update the behavior of the
    // entity: it is relevant because we just reached
    // the destination we previously picked.
    bool changed = behave(info, x, y);

    // Once this is done, we may need to emit a new
    // pheromon: this allow to make sure that we
    // indicate any change of state for this entity.
    if (changed) {
      emitPheromon(info);
    }
  }

  void
  HostileMob::chase(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement chase behavior.
  }

  void
  HostileMob::fight(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement fight behavior.
  }

  void
  HostileMob::collect(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement collect behavior.
  }

  void
  HostileMob::getBack(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement return behavior.
  }

  void
  HostileMob::wander(StepInfo& info, float& x, float& y) {
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
  }

}

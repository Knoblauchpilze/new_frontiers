
# include "Mob.hh"
# include "Locator.hh"

namespace new_frontiers {

  Mob::Mob(const MProps& props):
    Entity(props),

    m_homeX(props.homeX),
    m_homeY(props.homeY),

    m_carrying(std::max(std::min(props.cargo, props.carrying), 0.0f)),
    m_cargo(std::max(props.cargo, 0.0f)),

    m_energy(props.energy),
    m_energyRefill(props.refill),
    m_pheromonCost(props.pheromonCost),

    m_behavior(Behavior::Wander)
  {}

  bool
  Mob::takeAction(StepInfo& info, float& x, float& y) {
    // First, we need to update the behavior of the
    // entity: it is relevant because we just reached
    // the destination we previously picked.
    Thought t = behave(info);

    // Once this is done, we may need to emit a new
    // pheromon: this allow to make sure that we
    // indicate any change of state for this entity.
    if (t.behaviorChanged) {
      emitPheromon(info);
    }

    // Set the output position to the picked target
    // if any has been chosen.
    if (t.actionTaken) {
      x = t.xT;
      y = t.yT;
    }

    return t.actionTaken;
  }

  void
  Mob::pickRandomTarget(StepInfo& info, float& x, float& y) noexcept {
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

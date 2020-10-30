
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
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),
    m_pheromonCost(props.pheromonCost),

    m_behavior(Behavior::Wander)
  {}

  bool
  Mob::takeAction(StepInfo& info, path::Path& path) {
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
      m_path.clear(m_tile.x, m_tile.y);
      path.add(t.xT, t.yT);
    }

    return t.actionTaken;
  }

  void
  Mob::pickRandomTarget(StepInfo& info, float xS, float yS, float& x, float& y) noexcept {
    // Pick a random location within the radius of the
    // motion for this entity. We will use the locator
    // to determine if any element is obstructing the
    // view.
    // We want at least a path of `m_speed` long so as
    // to have a traversal duration of at least 1ms.
    // we also take a bit of margin for good measure.
    float r = info.rng.rndFloat(m_pathLength / 2.0f, m_pathLength);
    float theta = info.rng.rndAngle();

    float xDir = std::cos(theta);
    float yDir = std::sin(theta);

    // Clamp these coordinates and update the direction
    // based on that.
    info.clampPath(xS, yS, xDir, yDir, r);

    while (info.frustum->obstructed(xS, yS, xDir, yDir, r, m_path.cPoints)) {
      r = info.rng.rndFloat(m_pathLength / 2.0f, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      info.clampPath(xS, yS, xDir, yDir, r);
    }

    // Save the picked location.
    x = xS + r * xDir;
    y = yS + r * yDir;
  }

  Mob::Thought
  Mob::behave(StepInfo& info) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    Thought t{false, false, m_tile.x, m_tile.y};

    switch (s) {
      case Behavior::Chase:
        t.actionTaken = chase(info, t.xT, t.yT);
        break;
      case Behavior::Fight:
        t.actionTaken = fight(info, t.xT, t.yT);
        break;
      case Behavior::Collect:
        t.actionTaken = collect(info, t.xT, t.yT);
        break;
      case Behavior::Return:
        t.actionTaken = getBack(info, t.xT, t.yT);
        break;
      case Behavior::Wander:
      default:
        t.actionTaken = wander(info, t.xT, t.yT);
        break;
    }

    t.behaviorChanged = (s != m_behavior);

    return t;
  }

}


# include "Mob.hh"
# include "Locator.hh"

namespace new_frontiers {

  Mob::Mob(const MProps& props):
    Entity(props),

    m_home(Point{props.homeX, props.homeY}),

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
    Thought t = behave(info, path);

    // Once this is done, we may need to emit a new
    // pheromon: this allow to make sure that we
    // indicate any change of state for this entity.
    if (t.behaviorChanged) {
      emitPheromon(info);
    }

    // Set the output position to the picked target
    // if any has been chosen.
    if (t.actionTaken) {
      path = t.path;
    }

    return t.actionTaken;
  }

  void
  Mob::pickRandomTarget(StepInfo& info, const Point& r, float& x, float& y) noexcept {
    // Pick a random location within the radius of the
    // motion for this entity. We will use the locator
    // to determine if any element is obstructing the
    // view.
    // We want at least a path of `m_speed` long so as
    // to have a traversal duration of at least 1ms.
    // we also take a bit of margin for good measure.
    float d = info.rng.rndFloat(m_pathLength / 2.0f, m_pathLength);
    float theta = info.rng.rndAngle();

    float xDir = std::cos(theta);
    float yDir = std::sin(theta);

    // Clamp these coordinates and update the direction
    // based on that.
    info.clampPath(r, xDir, yDir, d);

    while (info.frustum->obstructed(r, xDir, yDir, d, m_path.cPoints)) {
      d = info.rng.rndFloat(m_pathLength / 2.0f, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      info.clampPath(r, xDir, yDir, d);
    }

    // Save the picked location.
    x = r.x + d * xDir;
    y = r.y + d * yDir;
  }

  Mob::Thought
  Mob::behave(StepInfo& info, const path::Path& /*path*/) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    Thought t;
    t.behaviorChanged = false;
    t.actionTaken = false;
    t.path = path::newPath(m_tile.p);

    switch (s) {
      case Behavior::Chase:
        t.actionTaken = chase(info, t.path);
        break;
      case Behavior::Fight:
        t.actionTaken = fight(info, t.path);
        break;
      case Behavior::Collect:
        t.actionTaken = collect(info, t.path);
        break;
      case Behavior::Return:
        t.actionTaken = getBack(info, t.path);
        break;
      case Behavior::Wander:
      default:
        t.actionTaken = wander(info, t.path);
        break;
    }

    t.behaviorChanged = (s != m_behavior);

    return t;
  }

}

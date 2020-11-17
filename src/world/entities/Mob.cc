
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

    Point t{r.x + d * xDir, r.y + d * yDir};

    while (info.frustum->obstructed(t)) {
      d = info.rng.rndFloat(m_pathLength / 2.0f, m_pathLength);
      theta = info.rng.rndAngle();

      xDir = std::cos(theta);
      yDir = std::sin(theta);

      // TODO: The fact that we clamp the coordinates
      // can lead to some issues in the path finding
      // as if we pick for example `(4, 5)` as target
      // the path is indeed not obstructed (the block
      // is at `(4, 4)` but we still can't go there
      // without going out of the world.
      // This should be resolved when we allow for an
      // infinite world.
      info.clampPath(r, xDir, yDir, d);
      t = Point{r.x + d * xDir, r.y + d * yDir};
    }

    // Save the picked location.
    x = r.x + d * xDir;
    y = r.y + d * yDir;
  }

  bool
  Mob::returnToWandering(StepInfo& info,
                         std::function<bool(VFXShPtr)> filter,
                         PheromonAnalyzer& analyzer,
                         path::Path& path)
  {
    // Collect the pheromons visible in the surroundings of
    // the entity: this will be filtered afterwards using
    // the provided function.
    tiles::Effect* te = nullptr;
    std::vector<VFXShPtr> allVFXs = info.frustum->getVisible(m_tile.p, m_perceptionRadius, te, -1, nullptr);

    std::vector<VFXShPtr> vfxs;
    for (unsigned id = 0u ; id < allVFXs.size() ; ++id) {
      if (!filter(allVFXs[id])) {
        vfxs.push_back(allVFXs[id]);
      }
    }

    // We will need a random target so better compute
    // it right now.
    float xRnd, yRnd;
    pickRandomTarget(info, m_tile.p, xRnd, yRnd);

    Point p{xRnd, yRnd};

    // Use the pheromons to select a biased random
    // target: this will allow to still have some
    // randomness but more directed towards what
    // other agents might have explored.
    if (!vfxs.empty()) {
      for (unsigned id = 0u ; id < vfxs.size() ; ++id) {
        VFXShPtr v = vfxs[id];

        PheromonShPtr p = std::dynamic_pointer_cast<Pheromon>(v);
        if (v == nullptr || p == nullptr) {
          continue;
        }

        analyzer.accumulate(*p);
      }

      // Use the pheromon analyze to yield a valid target
      // to pick for this warrior. The relative importance
      // of pheromons will be handled directly.
      analyzer.computeTarget(xRnd, yRnd);

      p.x = xRnd;
      p.y = yRnd;
    }

    // Reset the behavior and generate a path to the
    // target computed from visible pheromons.
    setBehavior(Behavior::Wander);
    return path.generatePathTo(info, p, false);
  }

  Mob::Thought
  Mob::behave(StepInfo& info, const path::Path& path) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    Thought t;
    t.behaviorChanged = false;
    t.actionTaken = false;
    t.path = path;

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
      case Behavior::Flee:
        t.actionTaken = flee(info, t.path);
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

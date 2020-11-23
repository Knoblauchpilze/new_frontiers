
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
                         path::Path& path,
                         unsigned attempts)
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

    // Accumulate the visible pheromons in the analyzer
    // to be able to pick a direction that is influenced
    // by them.
    if (!vfxs.empty()) {
      for (unsigned id = 0u ; id < vfxs.size() ; ++id) {
        VFXShPtr v = vfxs[id];

        PheromonShPtr p = std::dynamic_pointer_cast<Pheromon>(v);
        if (v == nullptr || p == nullptr) {
          continue;
        }

        analyzer.accumulate(*p);
      }
    }

    // Attempt to pick a random target (as the final
    // destination is biased to include some level of
    // randomness) and to generate a path to this pos.
    // If it fails, we will retry a certain number of
    // times before giving up.
    float xRnd, yRnd;
    Point p;
    bool generated = false;
    unsigned tries = 0u;
    path::Path newP = path::newPath(m_tile.p);

    while (!generated && tries < attempts) {
      pickRandomTarget(info, m_tile.p, xRnd, yRnd);
      analyzer.computeTarget(xRnd, yRnd);

      p.x = xRnd;
      p.y = yRnd;

      newP.clear(m_tile.p);
      generated = newP.generatePathTo(info, p, false, distance::d(m_tile.p, p) + 1.0f);
      ++tries;
    }

    // Whatever happens we will be in `Wander` behavior.
    setBehavior(Behavior::Wander);

    // In case the path could not be generated, wait.
    if (!generated) {
      log(
        "Failed to generate path from " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y) +
        " after " + std::to_string(tries) + " attempt(s)",
        utils::Level::Warning
      );

      return false;
    }

    std::swap(path, newP);
    return true;
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

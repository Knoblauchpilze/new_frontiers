
# include "Mob.hh"
# include "Locator.hh"
# include "../blocks/Deposit.hh"
# include "../blocks/SpawnerOMeter.hh"

namespace new_frontiers {

  Mob::Mob(const MProps& props):
    Entity(props),

    m_home(newPoint(props.homeX, props.homeY)),

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
      t = newPoint(r.x + d * xDir, r.y + d * yDir);
    }

    // Save the picked location.
    x = r.x + d * xDir;
    y = r.y + d * yDir;
  }

  bool
  Mob::wanderToDeposit(StepInfo& info, path::Path& path) noexcept {
    // Locate the closest deposit if any.
    BlockShPtr deposit = info.frustum->getClosest(path.cur, tiles::Portal, m_perceptionRadius, 14);
    DepositShPtr d = nullptr;
    if (deposit != nullptr) {
      d = std::dynamic_pointer_cast<Deposit>(deposit);
    }

    if (d == nullptr || d->getStock() <= 0.0f) {
      // No deposit could be found: we will
      // wander to try to find one or stick
      // with our current target if one is
      // already defined.
      if (isEnRoute()) {
        return false;
      }

      if (d != nullptr && d->getStock() <= 0.0f) {
        log("Deposit is empty");
      }

      pickTargetFromPheromon(info, path, Goal::Deposit);
      return true;
    }

    // Attempt to find a path to the deposit.
    Point p = d->getTile().p;
    p.x += 0.5f;
    p.y += 0.5f;

    log(
      "Entity at " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y) +
      " found deposit at " + std::to_string(p.x) + "x" + std::to_string(p.y) +
      " d: " + std::to_string(distance::d(m_tile.p, p))
    );

    if (!path.generatePathTo(info, p, true)) {
      return false;
    }

    // The path could be generated: return it
    // and set the behavior to `Collect`.
    setBehavior(Behavior::Collect);

    return true;
  }

  bool
  Mob::wanderToHome(StepInfo& info, path::Path& path) noexcept {
    // Locate the closest deposit if any.
    world::Filter f{getOwner(), true};
    BlockShPtr b = info.frustum->getClosest(m_tile.p, tiles::Portal, m_perceptionRadius, -1, &f);

    if (b == nullptr) {
      // The home can't be found: we will
      // wander to try to find it or stick
      // with our current target if one is
      // already defined.
      if (isEnRoute()) {
        return false;
      }

      log("Home is too far, can't see it");

      pickTargetFromPheromon(info, path, Goal::Home);
      return true;
    }

    // Attempt to find a path to the home.
    Point p = b->getTile().p;
    p.x += 0.5f;
    p.y += 0.5f;

    log(
      "Entity at " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y) +
      " found home at " + std::to_string(p.x) + "x" + std::to_string(p.y) +
      " d: " + std::to_string(distance::d(m_tile.p, p))
    );

    if (!path.generatePathTo(info, p, true)) {
      return false;
    }

    // The path could be generated: return it
    // and set the behavior to `Return`.
    setBehavior(Behavior::Return);

    return true;
  }

  bool
  Mob::wanderToEntity(StepInfo& info, path::Path& path) noexcept {
    // Locate the closest entity if any.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(m_tile.p, m_perceptionRadius, te, -1, &f, world::Sort::Distance);

    // In case there are no entities, continue the
    // wandering around process.
    if (entities.empty()) {
      // In case we already have a target, continue
      // towards this target.
      if (isEnRoute()) {
        return false;
      }

      log("Entity is home alone, no friends nearby");

      pickTargetFromPheromon(info, path, Goal::Entity);
      return true;
    }

    // Pick the first one as it will be the closest
    // and attempt to find a path to reach it.
    EntityShPtr e = entities.front();

    if (!path.generatePathTo(info, e->getTile().p, false)) {
      return false;
    }

    log(
      "Entity at " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y) +
      " found ennemy at " + std::to_string(e->getTile().p.x) + "x" + std::to_string(e->getTile().p.y) +
      " d: " + std::to_string(distance::d(m_tile.p, e->getTile().p))
    );

    // The path could be generated: return it
    // and set the behavior to `Chase`.
    setBehavior(Behavior::Chase);

    return true;
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

  void
  Mob::pickTargetFromPheromon(StepInfo& info,
                              path::Path& path,
                              const Goal& priority) noexcept
  {
    // Generate the pheromon analyzer and the
    // filtering method for pheromons.
    PheromonAnalyzer pa = generateFromGoal(priority);

    utils::Uuid owner = getOwner();
    auto filter = [&owner](VFXShPtr vfx) {
      return vfx->getOwner() != owner;
    };

    // Use the dedicated handler.
    if (!returnToWandering(info, filter, pa, path)) {
      log(
        "Unable to return to wandering, path could not be generated",
        utils::Level::Warning
      );
    }
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

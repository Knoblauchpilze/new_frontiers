
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "PheromonAnalyzer.hh"

namespace new_frontiers {

  Warrior::Warrior(const WProps& props):
    Mob(props),

    m_attack(props.attack),
    m_attackCost(props.attackCost)
  {
    setService("warrior");
  }

  bool
  Warrior::chase(StepInfo& info, path::Path& path) {
    // In chase mode we need to pick the closest entity
    // and set its current position as the new target
    // for this entity.
    // In case no entities can be found, we will return
    // to the wandering behavior.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(
      m_tile.x,
      m_tile.y,
      m_perceptionRadius,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    float x, y;

    if (entities.empty()) {
      // Couldn't find the entity we were chasing, get
      // back to wander behavior.
      log("Lost entity at " + std::to_string(m_tile.x) + "x" + std::to_string(m_tile.y));

      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);
      path.generatePathTo(info, x, y);

      return true;
    }

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Update the target with the actual position of
    // the entity: indeed the entity may be moving
    // so we want to accurately chase it.
    path.clear(m_tile.x, m_tile.y);
    path.generatePathTo(info, e->getTile().x, e->getTile().y);

    // In case we are close enough of the entity to
    // actually hit it, do so if we are able to.
    if (!isEnRoute() && m_energy >= m_attackCost) {
      bool alive = e->damage(m_attack);
      log("Attacking for " + std::to_string(m_attack) + " damage, " + std::to_string(e->getHealthRatio()) + " health ratio");

      m_energy -= m_attackCost;

      // Mark the entity for deletion in case it is
      // now dead. We will also return back to the
      // wandering behavior.
      if (!alive) {
        log("Killed entity at " + std::to_string(e->getTile().x) + "x" + std::to_string(e->getTile().y));

        info.removeEntity(e.get());

        setBehavior(Behavior::Wander);
        pickTargetFromPheromon(info, x, y);
        path.generatePathTo(info, x, y);

        return true;
      }
    }

    return true;
  }

  bool
  Warrior::getBack(StepInfo& info, path::Path& path) {
    // The get back behavior is active whenever the
    // warrior needs to get back home.
    if (isEnRoute()) {
      // Still en route, do nothing until we reach
      // home.
      return false;
    }

    float x, y;

    // We have reached home, do nothing and wait for
    // new instructions. We need to make sure that
    // we are actually close to home (and that it did
    // not get destroyed for some reasons).
    world::Filter f{getOwner(), true};
    world::ItemEntry ie = info.frustum->getClosest(m_tile.x, m_tile.y, world::ItemType::Block, f);
    world::Block b;

    if (ie.index >= 0 && ie.type == world::ItemType::Block) {
      b = info.frustum->block(ie.index);
    }

    if (ie.index < 0 || ie.type != world::ItemType::Block || b.tile.type != tiles::Portal) {
      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);
      path.generatePathTo(info, x, y);

      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Reached home, now retired");

    return false;
  }

  bool
  Warrior::wander(StepInfo& info, path::Path& path) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(
      m_tile.x,
      m_tile.y,
      m_perceptionRadius,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    // In case there are no entities, continue the
    // wandering around process.
    if (entities.empty()) {
      // In case we already have a target, continue
      // towards this target.
      if (isEnRoute()) {
        return false;
      }

      float x, y;
      pickTargetFromPheromon(info, x, y);
      path.generatePathTo(info, x, y);

      return true;
    }

    // Change to chasing behavior.
    setBehavior(Behavior::Chase);

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Assign the target to the closest entities:
    // as we requested the list to be sorted we
    // can pick the first one.
    path.generatePathTo(info, e->getTile().x, e->getTile().y);

    return true;
  }

  void
  Warrior::pickTargetFromPheromon(StepInfo& info, float& x, float& y) noexcept {
    // Collect the pheromons laid out by ennemis of
    // this warrior.
    world::Filter f{getOwner(), false};
    tiles::Effect* te = nullptr;
    std::vector<VFXShPtr> d = info.frustum->getVisible(m_tile.x, m_tile.y, m_perceptionRadius, te, -1, &f);

    // We will need a random target so better compute
    // it right now.
    float xRnd, yRnd;
    pickRandomTarget(info, m_tile.x, m_tile.y, xRnd, yRnd);

    // In case no pheromons are visible use the
    // default wandering behavior.
    if (d.empty()) {
      x = xRnd;
      y = yRnd;

      return;
    }

    // Otherwise, see what kind of pheromones are
    // visible: we will give some priority to the
    // fighting pheromon over other types and so
    // on to specialize the behavior.
    PheromonAnalyzer pa;
    pa.setRelativeWeight(pheromon::Type::Chase, 0.1f);
    pa.setRelativeWeight(pheromon::Type::Fight, 0.25f);
    pa.setRelativeWeight(pheromon::Type::Collect, 0.2f);
    pa.setRelativeWeight(pheromon::Type::Return, 0.15f);
    pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
    pa.setRandomWeight(0.3f);

    for (unsigned id = 0u ; id < d.size() ; ++id) {
      VFXShPtr v = d[id];

      PheromonShPtr p = std::dynamic_pointer_cast<Pheromon>(v);
      if (v == nullptr || p == nullptr) {
        continue;
      }

      pa.accumulate(*p);
    }

    // Use the pheromon analyze to yield a valid target
    // to pick for this warrior. The relative importance
    // of pheromons will be handled directly.
    pa.computeTarget(xRnd, yRnd);

    x = xRnd;
    y = yRnd;
  }
}

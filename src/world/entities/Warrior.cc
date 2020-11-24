
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "PheromonAnalyzer.hh"

namespace new_frontiers {

  Warrior::Warrior(const WProps& props):
    Mob(props),

    m_attack(props.attack),
    m_attackCost(props.attackCost),
    m_attackRange(props.attackRange)
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
      m_tile.p,
      m_perceptionRadius,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    if (entities.empty()) {
      // Couldn't find the entity we were chasing, get
      // back to wander behavior.
      log("Lost entity at " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y));

      pickTargetFromPheromon(info, path);
      return true;
    }

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Update the target with the actual position of
    // the entity: indeed the entity may be moving
    // so we want to accurately chase it.
    path.clear(m_tile.p);
    if (!path.generatePathTo(info, e->getTile().p, false)) {
      // Couldn't reach the entity, return to wandering.
      log("Entity is now unreachable, returning to wandering from " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y));
      pickTargetFromPheromon(info, path);
      return true;
    }

    // In case we are close enough of the entity to
    // actually hit it, do so if we are able to.
    if (m_energy >= m_attackCost && distance::d(e->getTile().p, m_tile.p) < m_attackRange) {
      bool alive = e->damage(m_attack);
      log("Attacking for " + std::to_string(m_attack) + " damage, " + std::to_string(e->getHealthRatio()) + " health ratio");

      m_energy -= m_attackCost;

      // Mark the entity for deletion in case it is
      // now dead. We will also return back to the
      // wandering behavior.
      if (!alive) {
        log("Killed entity at " + std::to_string(e->getTile().p.x) + "x" + std::to_string(e->getTile().p.y));

        info.removeEntity(e.get());
        pickTargetFromPheromon(info, path);
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

    // We have reached home, do nothing and wait for
    // new instructions. We need to make sure that
    // we are actually close to home (and that it did
    // not get destroyed for some reasons).
    world::Filter f{getOwner(), true};
    world::ItemEntry ie = info.frustum->getClosest(m_tile.p, world::ItemType::Block, f);
    world::Block b;

    if (ie.index >= 0 && ie.type == world::ItemType::Block) {
      b = info.frustum->block(ie.index);
    }

    if (ie.index < 0 || ie.type != world::ItemType::Block || b.tile.type != tiles::Portal) {
      pickTargetFromPheromon(info, path);
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
    // TODO: We should probably have some sort of
    // biased wandering as for the `Worker` class.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(
      m_tile.p,
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

      pickTargetFromPheromon(info, path);
      return true;
    }

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Try to go to this entity: if it fails we will
    // just continue on our current target.
    path::Path newPath = path::newPath(m_tile.p);
    std::swap(newPath, path);

    bool generated = newPath.generatePathTo(info, e->getTile().p, false);
    if (!generated) {
      if (isEnRoute()) {
        return false;
      }

      pickTargetFromPheromon(info, path);
      return true;
    }

    // We successfully generated a path to the entity,
    // use the chase mode.
    setBehavior(Behavior::Chase);
    std::swap(newPath, path);

    return generated;
  }

  void
  Warrior::pickTargetFromPheromon(StepInfo& info, path::Path& path) noexcept {
    // Generate the pheromon analyzer.
    PheromonAnalyzer pa;
    pa.setRelativeWeight(pheromon::Type::Chase, 0.1f);
    pa.setRelativeWeight(pheromon::Type::Fight, 0.25f);
    pa.setRelativeWeight(pheromon::Type::Collect, 0.2f);
    pa.setRelativeWeight(pheromon::Type::Return, 0.07f);
    pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
    pa.setRelativeWeight(pheromon::Type::Flee, 0.08f);
    pa.setRandomWeight(0.3f);

    // Generate the filtering method for pheromons.
    utils::Uuid owner = getOwner();
    auto filter = [&owner](VFXShPtr vfx) {
      return vfx->getOwner() == owner;
    };

    // Use the dedicated handler.
    if (!returnToWandering(info, filter, pa, path)) {
      log(
        "Unable to return to wandering, path could not be generated",
        utils::Level::Warning
      );
    }
  }
}

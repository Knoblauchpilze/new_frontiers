
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "PheromonAnalyzer.hh"
# include <cxxabi.h>

namespace new_frontiers {

  Warrior::Warrior(const WProps& props):
    Mob(props),

    m_attack(props.attack),
    m_attackCost(props.attackCost),
    m_attackRange(props.attackRange),

    m_seekForHealthThreshold(props.seekForHealth)
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

      pickTargetFromPheromon(info, path, Goal::Entity);
      return true;
    }

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Update the target with the actual position of
    // the entity: indeed the entity may be moving
    // so we want to accurately chase it.
    path.clear(m_tile.p);
    if (!path.generatePathTo(info, e->getTile().p, false, m_perceptionRadius)) {
      // Couldn't reach the entity, return to wandering.
      log("Entity is now unreachable, returning to wandering from " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y));
      pickTargetFromPheromon(info, path, Goal::Entity);
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

        // Now we would like to either get back to
        // the colony in case the entity has lost
        // too much life or continue searching for
        // ennemies to kill. This will be handled
        // better by the `wander` method.
        return wander(info, path);
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

    // We have reached home. Make sure that the
    // home still exists.
    world::Filter f{getOwner(), true};
    BlockShPtr b = info.frustum->getClosest(m_tile.p, tiles::Portal, -1, -1, &f);

    if (b == nullptr) {
      // For some reason the home of the entity does
      // not exist, return to wandering.
      pickTargetFromPheromon(info, path, Goal::Home);
      return true;
    }

    SpawnerOMeterShPtr s = std::dynamic_pointer_cast<SpawnerOMeter>(b);
    if (s == nullptr) {
      // Not able to convert to a valid spawner,
      // use wander again.
      int status;
      std::string bType = abi::__cxa_demangle(typeid(*b).name(), 0, 0, &status);

      log(
        "Reached block of type \"" + bType + "\" which is not a spawner in return behavior",
        utils::Level::Warning
      );

      pickTargetFromPheromon(info, path, Goal::Home);
      return true;
    }

    // Try to heal as much as possible in the limit of
    // our own health pool.
    float missing = m_totalHealth - m_health;
    float gain = s->refill(-missing, false);
    m_health += gain;

    log("Healed for " + std::to_string(gain) + " (" + std::to_string(missing) + " was missing)");
    // TODO: Maybe make sure that the home is able to
    // provide some heal: otherwise we should go back
    // to regular behavior.

    // Re-wander again.
    pickTargetFromPheromon(info, path, Goal::Entity);
    return true;
  }

  bool
  Warrior::wander(StepInfo& info, path::Path& path) {
    // Depending on the status of the entity we will
    // prioritize a wandering behavior.
    path::Path newPath = path::newPath(m_tile.p);
    bool generated = false;

    if (getHealthRatio() <= m_seekForHealthThreshold) {
      generated = wanderToHome(info, newPath);
    }
    else {
      generated = wanderToEntity(info, newPath);
    }

    if (generated) {
      std::swap(path, newPath);
      return true;
    }

    // We couldn't generate a path: either because
    // there is no target visible to the entity or
    // because the A* failed to find a route. In
    // both cases, the best bet is to continue with
    // the current path if any.
    if (isEnRoute()) {
      return false;
    }

    // We couldn't generate a path based on the
    // most rational behavior: use a random based
    // wandering for now.
    pickTargetFromPheromon(info, path, Goal::Random);
    return true;
  }

  PheromonAnalyzer
  Warrior::generateFromGoal(const Goal& goal) noexcept {
    PheromonAnalyzer pa;

    switch (goal) {
      case Goal::Deposit:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.4f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.5f);
        break;
      case Goal::Entity:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.25f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.2f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.07f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.08f);
        pa.setRandomWeight(0.3f);
        break;
      case Goal::Home:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.4f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.5f);
        break;
      case Goal::Random:
        // Assume default is also `Random`.
      default:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.05f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.05f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.1f);
        pa.setRandomWeight(0.8f);
        break;
    }

    return pa;
  }

}

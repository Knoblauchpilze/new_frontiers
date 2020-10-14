
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace new_frontiers {

  Warrior::Warrior(const WProps& props):
    Mob(props),

    m_attack(props.attack),

    m_attackDelay(props.attackDelay),
    m_last(now() - m_attackDelay),
    m_passed()
  {
    setService("warrior");
  }

  bool
  Warrior::chase(StepInfo& info, float& x, float& y) {
    // In chase mode we need to pick the closest entity
    // and set its current position as the new target
    // for this entity.
    // In case no entities can be found, we will return
    // to the wandering behavior.
    EntityShPtr e = info.frustum->getClosest(x, y, tiles::Executioner, m_perceptionRadius, 0);

    if (e == nullptr) {
      // Couldn't find the entity we were chasing, get
      // back to wander behavior.
      log("Lost entity at " + std::to_string(x) + "x" + std::to_string(y));

      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    // Update the target with the actual position of
    // the entity: indeed the entity may be moving
    // so we want to accurately chase it.
    x = e->getTile().x;
    y = e->getTile().y;

    // In case we are close enough of the entity to
    // actually hit it, do so if we are able to.
    if (!isEnRoute() && m_last + m_attackDelay <= info.moment) {
      bool alive = e->damage(m_attack);
      log("Attacking for " + std::to_string(m_attack) + " damage, " + std::to_string(e->getHealthRatio()) + " health ratio");

      m_last = info.moment;

      // Mark the entity for deletion in case it is
      // now dead. We will also return back to the
      // wandering behavior.
      if (!alive) {
        log("Killed entity at " + std::to_string(x) + "x" + std::to_string(y));

        info.removeEntity(e.get());

        setBehavior(Behavior::Wander);
        pickRandomTarget(info, x, y);

        return true;
      }
    }

    return true;
  }

  bool
  Warrior::getBack(StepInfo& info, float& x, float& y) {
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
    world::ItemEntry ie = info.frustum->getClosest(m_tile.x, m_tile.y, world::ItemType::Block);
    world::Block b;

    if (ie.index >= 0 && ie.type == world::ItemType::Block) {
      b = info.frustum->block(ie.index);
    }

    if (ie.index < 0 || ie.type != world::ItemType::Block || b.tile.type != tiles::Portal) {
      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Reached home, now retired");

    return false;
  }

  bool
  Warrior::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    EntityShPtr entity = info.frustum->getClosest(x, y, tiles::Executioner, m_perceptionRadius, 0);

    // In case there are no entities, continue the
    // wandering around process.
    if (entity == nullptr) {
      // In case we already have a target, continue
      // towards this target.
      if (isEnRoute()) {
        return false;
      }

      pickRandomTarget(info, x, y);

      return true;
    }

    // Change to chasing behavior.
    setBehavior(Behavior::Chase);

    // Assign the target to the closest entities:
    // as we requested the list to be sorted we
    // can pick the first one.
    x = entity->getTile().x;
    y = entity->getTile().y;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

}

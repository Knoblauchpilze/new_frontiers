#ifndef    ENTITY_FACTORY_HXX
# define   ENTITY_FACTORY_HXX

# include "EntityFactory.hxx"

namespace new_frontiers {

  inline
  Entity::Props
  EntityFactory::newEntityProps(float x, float y, const tiles::Entity& ent) noexcept {
    Entity::Props pp;

    pp.tile = newTile(ent, 0, x, y);
    pp.radius = sk_radius;

    pp.perception = sk_perception;
    pp.health = sk_health;

    pp.arrival = sk_arrival;
    pp.pathLength = sk_pathLength;

    pp.owner.invalidate();

    return pp;
  }

  inline
  Worker::WProps
  EntityFactory::newWorkerProps(float x, float y, const tiles::Entity& ent) noexcept {
    Worker::WProps pp;

    pp.tile = newTile(ent, 0, x, y);
    pp.radius = sk_radius;

    pp.perception = sk_perception;
    pp.health = sk_health;

    pp.arrival = sk_arrival;
    pp.pathLength = sk_pathLength;

    pp.owner.invalidate();

    pp.homeX = x;
    pp.homeY = y;

    pp.carrying = 0.0f;
    pp.cargo = 10.0f;

    pp.energy = 1.9f;
    pp.maxEnergy = 2.1f;
    pp.refill = 1.0f;
    pp.pheromonCost = 2.0f;

    pp.fleeRadius = 1.5f;
    pp.fleeConeSpan = 3.1415926535f / 4.0f;

    return pp;
  }

  inline
  Warrior::WProps
  EntityFactory::newWarriorProps(float x, float y, const tiles::Entity& ent) noexcept {
    Warrior::WProps pp;

    pp.tile = newTile(ent, 0, x, y);
    pp.radius = sk_radius;

    pp.perception = sk_perception;
    pp.health = sk_health;

    pp.arrival = sk_arrival;
    pp.pathLength = sk_pathLength;

    pp.owner.invalidate();

    pp.homeX = x;
    pp.homeY = y;

    pp.carrying = 0.0f;
    pp.cargo = 10.0f;

    pp.energy = 1.9f;
    pp.maxEnergy = 2.1f;
    pp.refill = 1.0f;
    pp.pheromonCost = 2.0f;

    pp.attack = 2.0f;
    pp.attackCost = 1.0f;
    pp.attackRange = 0.25f;

    pp.seekForHealth = 0.2f;

    return pp;
  }

  inline
  Player::PProps
  EntityFactory::newPlayerProps(float x, float y, const tiles::Entity& ent) noexcept {
    Player::PProps pp;

    pp.tile = newTile(ent, 0, x, y);
    pp.radius = 2.0f * sk_radius;

    pp.perception = sk_perception;
    pp.health = sk_health;

    pp.arrival = sk_arrival;
    pp.pathLength = sk_pathLength;

    pp.owner.invalidate();

    pp.sprintSpeed = 4.0f;
    pp.recoverySpeed = 0.5f;

    pp.exhaustion = utils::toMilliseconds(3000);
    pp.recovery = utils::toMilliseconds(6000);

    return pp;
  }

  inline
  EntityShPtr
  EntityFactory::newEntityFromProps(const Entity::Props& props) noexcept {
    // Attempt to convert the input properties into
    // known types. Note that we should perform the
    // most derived types first otherwise we won't
    // be able to get the most precise match.
    const Entity::Props* pp = &props;

    const Warrior::WProps* wp = dynamic_cast<const Warrior::WProps*>(pp);
    if (wp != nullptr) {
      return std::make_shared<Warrior>(*wp);
    }

    const Worker::WProps* wp2 = dynamic_cast<const Worker::WProps*>(pp);
    if (wp2 != nullptr) {
      return std::make_shared<Worker>(*wp2);
    }

    const Player::PProps* plp = dynamic_cast<const Player::PProps*>(pp);
    if (plp != nullptr) {
      return std::make_shared<Player>(*plp);
    }

    // Can't interpret the props, return an invalid
    // pointer to an entity.
    return nullptr;
  }

  inline
  EntityTile
  EntityFactory::newTile(const tiles::Entity& e, int id, float x, float y) noexcept {
    return EntityTile{utils::Point2f(x, y), e, id};
  }

}

#endif    /* ENTITY_FACTORY_HXX */

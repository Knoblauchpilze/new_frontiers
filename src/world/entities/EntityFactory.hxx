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
  Worker::MProps
  EntityFactory::newWorkerProps(float x, float y, const tiles::Entity& ent) noexcept {
    Worker::MProps pp;

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

    pp.exhaustion = toMilliseconds(3000);
    pp.recovery = toMilliseconds(6000);

    return pp;
  }

  inline
  EntityTile
  EntityFactory::newTile(const tiles::Entity& e, int id, float x, float y) noexcept {
    return EntityTile{x, y, e, id};
  }

}

#endif    /* ENTITY_FACTORY_HXX */

#ifndef    ENTITY_FACTORY_HH
# define   ENTITY_FACTORY_HH

# include "Tiles.hh"
# include "Entity.hh"
# include "Worker.hh"
# include "Warrior.hh"
# include "Player.hh"

namespace new_frontiers {

  class EntityFactory {
    public:

      static
      Entity::Props
      newEntityProps(float x, float y, const tiles::Entity& ent) noexcept;

      static
      Worker::MProps
      newWorkerProps(float x, float y, const tiles::Entity& ent) noexcept;

      static
      Warrior::WProps
      newWarriorProps(float x, float y, const tiles::Entity& ent) noexcept;

      static
      Player::PProps
      newPlayerProps(float x, float y, const tiles::Entity& ent) noexcept;

    private:

      static
      EntityTile
      newTile(const tiles::Entity& e, int id, float x, float y) noexcept;

    private:

      static const float sk_radius;

      static const float sk_perception;
      static const float sk_health;

      static const float sk_arrival;
      static const float sk_pathLength;
  };

}

# include "EntityFactory.hxx"

#endif    /* ENTITY_FACTORY_HH */

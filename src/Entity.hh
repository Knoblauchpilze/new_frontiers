#ifndef    ENTITY_HH
# define   ENTITY_HH

# include <memory>
# include "WorldElement.hh"
# include "VFX.hh"

namespace new_frontiers {

  class Entity: public WorldElement<Mob> {
    public:

      /**
       * @brief - Creates a new entity with the specified tile
       *          and VFX produced.
       * @param tile - the visual representation of the entity
       *               along with its position.
       * @param vfx - the type of visual effects that the entity
       *              is able to produce.
       */
      Entity(const EntityTile& tile,
             const Effect& vfx);

      /**
       * @brief - Implementation of the interface method to evolve
       *          this entity. It will mostly handle the motion of
       *          the entity and potential creation of VFX.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      virtual bool
      step(StepInfo& info);

    private:

      /** TODO: Comment. **/
      float m_ox;
      float m_oy;

      bool m_started;
      float m_radius;
      Duration m_lap;
      TimeStamp m_init;

      Effect m_vfx;

      Duration m_vfxDelta;
      TimeStamp m_last;
  };

  using EntityShPtr = std::shared_ptr<Entity>;
}

# include "Entity.hxx"

#endif    /* ENTITY_HH */

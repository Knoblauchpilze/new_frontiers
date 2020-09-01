#ifndef    HOSTILE_MOB_HH
# define   HOSTILE_MOB_HH

# include "Entity.hh"

namespace new_frontiers {

  class HostileMob: public Entity {
    public:

      /**
       * @brief - Creates a new mob with the specified tile
       *          and a default VFX to emit.
       * @param tile - the visual representation of the entity
       *               along with its position.
       */
      HostileMob(const EntityTile& tile);

      /**
       * @brief - Override of the base class method to provide
       *          emission of a VFX at regular intervals.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      virtual bool
      step(StepInfo& info);

    public:

      /**
       * @brief - Duration between two consecutives emission of
       *          a VFX by this mob.
       */
      Duration m_vfxDelay;

      /**
       * @brief - The timestamp of the last time this entity has
       *          emitted a VFX.
       */
      TimeStamp m_last;
  };

  using HostileMobShPtr = std::shared_ptr<HostileMob>;
}

# include "HostileMob.hxx"

#endif    /* HOSTILE_MOB_HH */

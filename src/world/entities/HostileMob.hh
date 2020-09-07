#ifndef    HOSTILE_MOB_HH
# define   HOSTILE_MOB_HH

# include "Entity.hh"
# include "../effects/Pheromon.hh"

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
       * @brief - Implementation of the interface method to pause
       *          the internal processes for this mob. It mostly
       *          include the VFX spawning routine.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to resume
       *          the internal processes for this mob. We will be
       *          resuming the VFX spawning routine.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

      /**
       * @brief - Implementation of the interface method to select
       *          a speed for this entity.
       * @param info - an object describing (among other things) a
       *               rng.
       */
      void
      prepareForStep(const StepInfo& info) override;

      /**
       * @brief - Implementation of the interface method to select
       *          the target for this mob. We will use behaviors
       *          defined for the entity to choose.
       * @param info - info to pick the target.
       * @param x - the output abscissa of the chosen location.
       * @param y - the output ordinate of the chosen location.
       */
      void
      takeAction(StepInfo& info, float& x, float& y) override;

      /**
       * @brief - Implementation of the interface method to handle
       *          the creation of pheromons by this entity.
       * @param info - info to handle creation of pheromons.
       */
      void
      postStep(StepInfo& info) override;

    private:

      /**
       * @brief - Defines the mode in which this entity is currently
       *          in. The mode defins which type of behavior it will
       *          exhibit. Behaviors are controlled by the elements
       *          visible in the field of view of the entity.
       */
      enum class Behavior {
        Wander,
        Chase,
        Fight,
        Collect,
        Return
      };

      static
      pheromon::Type
      behaviorToPheromon(const Behavior& b) noexcept;

      void
      emitPheromon(StepInfo& info) noexcept;

      /**
       * @brief - Dispatch method which will analyze the current state
       *          of the entity and select the appropriate behavior to
       *          execute.
       * @param info - information about the surroundings of the mob.
       * @return -  `true` if the behavior of the entity was changed
       *            by this function. It allows to react when an entity
       *            find an enemy, etc.
       */
      bool
      behave(StepInfo& info, float& x, float&y) noexcept;

      void
      chase(StepInfo& info, float& x, float&y);

      void
      fight(StepInfo& info, float& x, float&y);

      void
      collect(StepInfo& info, float& x, float&y);

      void
      getBack(StepInfo& info, float& x, float&y);

      void
      wander(StepInfo& info, float& x, float&y);

    private:

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

      /**
       * @brief - Hold the duration that has passed since the last
       *          time a vfx was emitted in case a pause event is
       *          received.
       *          Used to be able to restore the vfx production as
       *          if nothins happened.
       */
      Duration m_passed;

      /**
       * @brief - Describe the current behavior for this entity. It
       *          is changed dynamically based on the surrounding
       *          elements.
       *          Each behavior will lead to different actions that
       *          can be performed by the entity.
       */
      Behavior m_behavior;
  };

  using HostileMobShPtr = std::shared_ptr<HostileMob>;
}

# include "HostileMob.hxx"

#endif    /* HOSTILE_MOB_HH */
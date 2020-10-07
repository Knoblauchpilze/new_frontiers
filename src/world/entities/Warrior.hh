#ifndef    WARRIOR_HH
# define   WARRIOR_HH

# include "Mob.hh"

namespace new_frontiers {

  class Warrior: public Mob {
    public:

      /**
       * @brief - Creates a new mob with the specified tile.
       * @param tile - the visual representation of the entity
       *               along with its position.
       * @param homeX - the abscissa of the home position for
       *                this mob.
       * @param homeY - the abscissa of the home position for
       *                this mob.
       */
      Warrior(const EntityTile& tile,
              float homeX,
              float homeY);

      /**
       * @brief - Implementation of the interface method to pause
       *          the internal processes for this warrior. It is
       *          mostly linked to the attacking process.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to resume
       *          the internal processes for this warrior.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

      bool
      chase(StepInfo& info, float& x, float& y) override;

      bool
      fight(StepInfo& info, float& x, float& y) override;

      bool
      getBack(StepInfo& info, float& x, float& y) override;

      bool
      wander(StepInfo& info, float& x, float& y) override;

    private:

      /**
       * @brief - The damage value for this warrior. Each
       *          attack will inflict that many damage.
       */
      float m_attack;

      /**
       * @brief - The duration between two consecutive
       *          attacks given other conditions are met.
       *          Expressed in milliseconds.
       */
      Duration m_attackDelay;

      /**
       * @brief - The timestamp at which the last attack
       *          took place. Allow to make sure that we
       *          do not attack too often.
       */
      TimeStamp m_last;

      /**
       * @brief - Hold the duration that has passed since
       *          the last time an attack was performed in
       *          case a pause event is received.
       *          Used to be able to restore the attacking
       *          behavior as if nothing happened.
       */
      Duration m_passed;
  };

  using WarriorShPtr = std::shared_ptr<Warrior>;
}

# include "Warrior.hxx"

#endif    /* WARRIOR_HH */

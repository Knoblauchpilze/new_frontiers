#ifndef    WARRIOR_HH
# define   WARRIOR_HH

# include "Mob.hh"

namespace new_frontiers {

  class Warrior: public Mob {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create an entity. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct WProps: MProps {
        float attack;
        float attackCost;
      };

      /**
       * @brief - Creates a new mob with the specified tile.
       * @param props - the properties describing this mob.
       */
      Warrior(const WProps& props);

    protected:

      bool
      chase(StepInfo& info, float& x, float& y) override;

      bool
      getBack(StepInfo& info, float& x, float& y) override;

      bool
      wander(StepInfo& info, float& x, float& y) override;

    private:

      /**
       * @brief - Used to pick a semi-random target based on
       *          the pheromons that are visible from the
       *          position of this warrior.
       *          We don't avoid purely random behavior but
       *          rather weigh it with some deterministic
       *          behavior to favorize emergent behaviors
       *          where pheromons laid out by other mobs
       *          might lead to their 'owners'.
       * @param info - information about the surroundings of
       *               the warrior.
       * @param x - the abscissa of the output position.
       * @param y - the ordinate of the output position.
       */
      void
      pickTargetFromPheromon(StepInfo& info, float& x, float& y) noexcept;

    private:

      /**
       * @brief - The damage value for this warrior. Each
       *          attack will inflict that many damage.
       */
      float m_attack;

      /**
       * @brief - The cost in energy for an attack. It is
       *          meant as a way to integrate the attack
       *          in the thinking process of the warrior
       *          and thus make some sort of concurrency
       *          between the pheromon spawning and the
       *          attack.
       */
      float m_attackCost;
  };

  using WarriorShPtr = std::shared_ptr<Warrior>;
}

# include "Warrior.hxx"

#endif    /* WARRIOR_HH */

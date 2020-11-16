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
      chase(StepInfo& info, path::Path& path) override;

      bool
      getBack(StepInfo& info, path::Path& path) override;

      bool
      wander(StepInfo& info, path::Path& path) override;

      /**
       * @brief - Used to inhibit pheromon emission when the
       *          warrior is in wander mode.
       * @param info - not used by this method.
       * @return - `true` if the pheromon should be inhibited.
       */
      bool
      inhibitPheromon(StepInfo& info) const noexcept override;

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
       *          This method will use the base class method
       *          and setup the correct environnment for its
       *          execution.
       * @param info - information about the surroundings of
       *               the warrior.
       * @param path - the path to generate.
       */
      void
      pickTargetFromPheromon(StepInfo& info, path::Path& path) noexcept;

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

#ifndef    MOB_HH
# define   MOB_HH

# include "Entity.hh"
# include "../effects/Pheromon.hh"

namespace new_frontiers {

  class Mob: public Entity {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a mob. It is meant as a
       *          way to reduce the number of arguments that
       *          are needed for building an instance of this
       *          class.
       */
      struct MProps: Props {
        float homeX;
        float homeY;

        float carrying;
        float cargo;

        float energy;
        float maxEnergy;
        float refill;

        float pheromonCost;
      };

      /**
       * @brief - Creates a new mob with the specified tile and
       *          home position.
       * @param props - the properties describing this mob.
       */
      Mob(const MProps& props);

      float
      getCarryingCapacity() const noexcept;

      float
      getCarried() const noexcept;

      /**
       * @brief - Implementation of the interface method to pause
       *          the internal processes for this mob. Note that
       *          it is mostly provided so that this class is not
       *          abstract as there are not really any process to
       *          pause at this time.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to resume
       *          the internal processes for this mob.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

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

      /**
       * @brief - Define a new behavior as provided in input and
       *          assign it as the current behavior. No checks
       *          are performed so any assignment is valid.
       * @param b - the new behavior to define.
       */
      void
      setBehavior(const Behavior& b) noexcept;

      /**
       * @brief - Used to register the `cargo` as a carried amount
       *          for this mob. We check against the amount already
       *          carried and return what could not be registered.
       * @param cargo - the amount of the resource that should be
       *                additionally carried by the mob.
       * @return - a value representing the amount that cannot be
       *           carried by the mob.
       */
      float
      carry(float cargo) noexcept;

      float
      availableCargo() const noexcept;

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
       * @param path - the path to populate by this method.
       * @return - `true` if the mob took an action.
       */
      bool
      takeAction(StepInfo& info, path::Path& path) override;

      /**
       * @brief - Implementation of the interface method to handle
       *          the creation of pheromons by this entity.
       * @param info - info to handle creation of pheromons.
       */
      void
      postStep(StepInfo& info) override;

      /**
       * @brief - Method guaranteed to be called upon each
       *          invocation of the `emitPheromon` methos
       *          and which gives inheriting classes the
       *          opportunity to block emission of pheromon
       *          for some reason.
       *          The default implementation never inhibits
       *          pheromons.
       * @param info - information about the state of the
       *               world and the mob.
       * @return - `true` if the pheromon should be inhibited
       *           (and thus not produced).
       */
      virtual bool
      inhibitPheromon(StepInfo& info) const noexcept;

      virtual bool
      chase(StepInfo& info, float& x, float& y);

      virtual bool
      fight(StepInfo& info, float& x, float& y);

      virtual bool
      collect(StepInfo& info, float& x, float& y);

      virtual bool
      getBack(StepInfo& info, float& x, float& y);

      virtual bool
      wander(StepInfo& info, float& x, float& y);

      /**
       * @brief - Interface method used to pick a random target
       *          from which an unobstructed path can be drawn
       *          starting at the input position.
       * @param info - a random number generator.
       * @param xS - the starting abscissa for the random target.
       * @param yS - the starting ordinate for the random target.
       * @param x - the target abscissa of the mob.
       * @param y - the target ordinate of the mob.
       */
      void
      pickRandomTarget(StepInfo& info, float xS, float yS, float& x, float& y) noexcept;

    private:

      /**
       * @brief - Convenience structure to regroup the result of
       *          the thinking process of this mob.
       */
      struct Thought {
        bool behaviorChanged;
        bool actionTaken;

        float xT;
        float yT;
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
       * @return - a coherent thought result for this mob.
       */
      Thought
      behave(StepInfo& info) noexcept;

    protected:

      /**
       * @brief - The home abscissa of this mob. Used when the
       *          entity needs to come back home.
       */
      float m_homeX;

      /**
       * @brief - Ordniate of the home position of the mob.
       */
      float m_homeY;

      /**
       * @brief - The amount of a resource that is carried by
       *          the mob. This value is guaranteed to be at
       *          least `0`.
       */
      float m_carrying;

      /**
       * @brief - The maximum carrying capacity for this mob
       *          for any resource.
       *          At any moment we have `m_carrying <= m_cargo`.
       */
      float m_cargo;

      /**
       * @brief - An indication of the energy left for this
       *          mob to take actions. This include spawning
       *          new pheromones for example.
       */
      float m_energy;

      /**
       * @brief - Measure the maximum energy that this mob can
       *          have. Above this value the energy won't be
       *          refilled anymore.
       */
      float m_maxEnergy;

      /**
       * @brief - Indication of how fast the energy pool of
       *          this entity can be refilled over time. A
       *          faster refill rate indicates an entity that
       *          can take more decisions.
       */
      float m_energyRefill;

      /**
       * @brief - The cost in energy of spawning a phgeromon.
       */
      float m_pheromonCost;

      /**
       * @brief - Describe the current behavior for this entity. It
       *          is changed dynamically based on the surrounding
       *          elements.
       *          Each behavior will lead to different actions that
       *          can be performed by the entity.
       */
      Behavior m_behavior;
  };

  using MobShPtr = std::shared_ptr<Mob>;
}

# include "Mob.hxx"

#endif    /* MOB_HH */

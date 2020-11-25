#ifndef    MOB_HH
# define   MOB_HH

# include "Entity.hh"
# include "Point.hh"
# include "../effects/Pheromon.hh"
# include "PheromonAnalyzer.hh"
# include "Path.hh"

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
        Return,
        Flee
      };

      /**
       * @brief - Possible weigthing of pheromons as we might want to
       *          give priority to some specific types when wandering.
       */
      enum class Goal {
        Deposit,
        Entity,
        Home,
        Random
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
       *          invocation of the `emitPheromon` methods
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
      chase(StepInfo& info, path::Path& path);

      virtual bool
      fight(StepInfo& info, path::Path& path);

      virtual bool
      collect(StepInfo& info, path::Path& path);

      virtual bool
      getBack(StepInfo& info, path::Path& path);

      virtual bool
      flee(StepInfo& info, path::Path& path);

      virtual bool
      wander(StepInfo& info, path::Path& path);

      /**
       * @brief - Interface method used to pick a random target
       *          from which an unobstructed path can be drawn
       *          starting at the input position.
       * @param info - a random number generator.
       * @param r - the position for the random target.
       * @param x - the target abscissa of the mob.
       * @param y - the target ordinate of the mob.
       */
      void
      pickRandomTarget(StepInfo& info, const Point& r, float& x, float& y) noexcept;

      /**
       * @brief - Used to wander with the objective to reach
       *          a deposit. This will give higher priority
       *          to collect pheromons.
       * @param info - the information about the surroundings
       *               of the entity.
       * @param path - the path to generate. Note that this
       *               path is assumed to refer to a clean
       *               path: any result of the wandering is
       *               added to it. In case this method is
       *               returning `false`, the path is left
       *               in an undefined state.
       * @return - `true` in case a path could be generated.
       */
      bool
      wanderToDeposit(StepInfo& info, path::Path& path) noexcept;

      /**
       * @brief - Similar to the above method but performs
       *          a wandering biased towards finding the
       *          home of the entity.
       * @param info - the information about the surroundings
       *               of the entity.
       * @param path - the path to generate. Note that this
       *               path is assumed to refer to a clean
       *               path: any result of the wandering is
       *               added to it. In case this method is
       *               returning `false`, the path is left
       *               in an undefined state.
       * @return - `true` in case a path could be generated.
       */
      bool
      wanderToHome(StepInfo& info, path::Path& path) noexcept;

      /**
       * @brief - Similar to the above method but performs
       *          a wandering biased towards finding other
       *          entities.
       * @param info - the information about the surroundings
       *               of the entity.
       * @param path - the path to generate. Note that this
       *               path is assumed to refer to a clean
       *               path: any result of the wandering is
       *               added to it. In case this method is
       *               returning `false`, the path is left
       *               in an undefined state.
       * @return - `true` in case a path could be generated.
       */
      bool
      wanderToEntity(StepInfo& info, path::Path& path) noexcept;

      /**
       * @brief - Common handler that can be used during the
       *          though process of an agent. It will reset
       *          the active behavior to wandering and pick
       *          a random target based on the pheromons that
       *          are currently visible.
       *          The user can specify a filter that will be
       *          used to control which pheromons are used
       *          in the determination of the wandering
       *          target. The method also expects to be fed
       *          a way to weigh the pheromons that can be
       *          seen by the agent.
       * @param info - the information describing the world.
       * @param filter - a way to check whether a pheromon
       *                 is relevant for the wandering target.
       *                 It should return `true` in case the
       *                 pheromon is not relevant.
       * @param analyzer - a way to weigh the pheromons based
       *                   on their type.
       * @param path - the path to generate: any information
       *               will be added to this object.
       * @param attempts - define how many attempts can be made
       *                   before giving up on generating a new
       *                   wandering target.
       * @return - `true` in case a wandering target could be
       *           selected and `false` in case the generation
       *           failed for some reason (mostly due to the
       *           impossibility to reach the target from the
       *           current position).
       */
      bool
      returnToWandering(StepInfo& info,
                        std::function<bool(VFXShPtr)> filter,
                        PheromonAnalyzer& analyzer,
                        path::Path& path,
                        unsigned attempts = 10u);

      /**
       * @brief - Used to pick a semi-random target based on
       *          the pheromons that are visible from the
       *          position of this worker.
       *          We don't avoid purely random behavior but
       *          rather weigh it with some deterministic
       *          behavior to favorize emergent behaviors
       *          where another worker might have tracked
       *          a deposit before.
       *          This method will use the base class method
       *          and setup the correct environnment for its
       *          execution.
       * @param info - information about the surroundings of
       *               the worker.
       * @param path - the path to generate.
       * @param priority - the current priority: used to set
       *                   relative weights for pheromons.
       */
      void
      pickTargetFromPheromon(StepInfo& info,
                             path::Path& path,
                             const Goal& priority) noexcept;

      /**
       * @brief - Used to generate a pheromon analyzer to be
       *          able to weigh pheromons based on their type
       *          according to a priority (or goal). This is
       *          used during the wandering process to choose
       *          which pheromones are relevant according to
       *          the goal of the mob.
       * @param goal - the goal of the wandering process.
       * @return - a pheromon analyzer that will be able to
       *           weigh the pheromons based on the goal.
       */
      virtual PheromonAnalyzer
      generateFromGoal(const Goal& goal) noexcept = 0;

    private:

      /**
       * @brief - Convenience structure to regroup the result of
       *          the thinking process of this mob.
       */
      struct Thought {
        bool behaviorChanged;
        bool actionTaken;

        path::Path path;
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
       * @param path - the current path followed by the entity.
       * @return - a coherent thought result for this mob.
       */
      Thought
      behave(StepInfo& info, const path::Path& path) noexcept;

    protected:

      /**
       * @brief - The home position of this mob. Used when the
       *          entity needs to come back home.
       */
      Point m_home;

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

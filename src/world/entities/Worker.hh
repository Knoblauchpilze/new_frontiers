#ifndef    WORKER_HH
# define   WORKER_HH

# include "Mob.hh"

namespace new_frontiers {

  class Worker: public Mob {
    public:

      /**
       * @brief - Creates a new mob with the specified props.
       * @param props - the properties describing this mob.
       */
      Worker(const MProps& props);

    protected:

      bool
      collect(StepInfo& info, path::Path& path) override;

      bool
      getBack(StepInfo& info, path::Path& path) override;

      bool
      flee(StepInfo& info, path::Path& path) override;

      bool
      wander(StepInfo& info, path::Path& path) override;

      /**
       * @brief - Used to inhibit pheromon emission when the
       *          worker is in wander mode.
       * @param info - not used by this method.
       * @return - `true` if the pheromon should be inhibited.
       */
      bool
      inhibitPheromon(StepInfo& info) const noexcept override;

    private:

      /**
       * @brief - Possible weigthing of pheromons as
       *          we might want to give priority to
       *          some specific types.
       */
      enum class GoalPriority {
        Deposit,
        Home,
        Random
      };

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
                             const GoalPriority& priority) noexcept;

      /**
       * @brief - Used to wander with the objective to reach
       *          a deposit. This will give higher priority
       *          to collect pheromons.
       * @param info - the information about the surroundings
       *               of the entity.
       * @param path - the path to generate. Note that this
       *               path is assumed to refer to a clean
       *               path: any result of the wandering is
       *               added to it. In case thie method is
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
       *               added to it. In case thie method is
       *               returning `false`, the path is left
       *               in an undefined state.
       * @return - `true` in case a path could be generated.
       */
      bool
      wanderToHome(StepInfo& info, path::Path& path) noexcept;

      /**
       * @brief - Used to verify in each behavior whether we
       *          can see some enemies warriors: if this is
       *          the case we should try to flee them so as
       *          not to end up dead.
       * @param info - information about the world.
       * @param path - the path to update in case a decision
       *               is taken.
       * @return - `true` if a decision was made: this means
       *           that no further processing is required
       *           for the time being.
       */
      bool
      checkForFlee(StepInfo& info, path::Path& path) noexcept;
  };

  using WorkerShPtr = std::shared_ptr<Worker>;
}

# include "Worker.hxx"

#endif    /* WORKER_HH */

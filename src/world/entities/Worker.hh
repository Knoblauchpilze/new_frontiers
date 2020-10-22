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
      collect(StepInfo& info, float& x, float& y) override;

      bool
      getBack(StepInfo& info, float& x, float& y) override;

      bool
      wander(StepInfo& info, float& x, float& y) override;

    private:

      /**
       * @brief - Used to pick a semi-random target based on
       *          the pheromons that are visible from the
       *          position of this worker.
       *          We don't avoid purely random behavior but
       *          rather weigh it with some deterministic
       *          behavior to favorize emergent behaviors
       *          where another worker might have tracked
       *          a deposit before.
       * @param info - information about the surroundings of
       *               the worker.
       * @param x - the abscissa of the output position.
       * @param y - the ordinate of the output position.
       */
      void
      pickTargetFromPheromon(StepInfo& info, float& x, float& y) noexcept;
  };

  using WorkerShPtr = std::shared_ptr<Worker>;
}

# include "Worker.hxx"

#endif    /* WORKER_HH */

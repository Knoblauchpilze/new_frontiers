#ifndef    WORKER_HH
# define   WORKER_HH

# include "Mob.hh"

namespace new_frontiers {

  class Worker: public Mob {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a worker. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct WProps: MProps {
        float fleeRadius;
        float fleeConeSpan;
      };

      /**
       * @brief - Creates a new mob with the specified props.
       * @param props - the properties describing this mob.
       */
      Worker(const WProps& props);

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

      PheromonAnalyzer
      generateFromGoal(const Goal& goal) noexcept override;

    private:

      /**
       * @brief - Used to verify in each behavior whether we
       *          can see some enemies warriors: if this is
       *          the case we should try to flee them so as
       *          not to end up dead.
       * @param info - information about the world.
       * @param path - the path to update in case a decision
       *               is taken.
       * @param pathChanged - `true` in case the path has
       *                      been changed compared to what
       *                      it was.
       *                      This value should be used as
       *                      the return value in case the
       *                      return of this function is set
       *                      to `true` to indicate to the
       *                      behavior processing layer the
       *                      status of the path.
       * @return - `true` if a decision was made: this means
       *           that no further processing is required
       *           for the time being.
       */
      bool
      checkForFlee(StepInfo& info, path::Path& path, bool& pathChanged) noexcept;

    private:

      /**
       * @brief - This valus is used as a threshold to consider
       *          that an enemy mob is close enough to us that
       *          it doesn't matter if it gets closer. It allows
       *          to keep the weights in the flee behavior from
       *          going to infinity.
       */
      static constexpr float sk_proximityAlert = 0.01f;

      /**
       * @brief - Defines the radius below which an entity
       *          considers that it should flee.
       */
      float m_fleeRadiusThreshold;

      /**
       * @brief - Defines the width of the cone where the entity
       *          is attempting to flee when enemies are detected.
       *          This value is expressed in radians.
       *          The larger this value the wider the cone to flee
       *          will be, giving the entity less of an urge to
       *          flee.
       */
      float m_fleeConeAngleSpan;
  };

  using WorkerShPtr = std::shared_ptr<Worker>;
}

# include "Worker.hxx"

#endif    /* WORKER_HH */

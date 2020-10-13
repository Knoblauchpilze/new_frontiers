#ifndef    TIMED_SPAWNER_HH
# define   TIMED_SPAWNER_HH

# include <memory>
# include "Spawner.hh"

namespace new_frontiers {

  class TimedSpawner: public Spawner {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a spawner. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct TSProps: public SProps {
        Duration interval;
        int stock;
      };

      /**
       * @brief - Create a new portal that is able to spawn new
       *          entities on a countdown. The amount of entities
       *          that can be spawned can be limited if needed.
       * @param props - the description of the properties to use
       *                when instantiating the spawner.
       */
      TimedSpawner(const TSProps& props);

      void
      pause(const TimeStamp& t) override;

      void
      resume(const TimeStamp& t) override;

    protected:

      void
      update(StepInfo& info) override;

      bool
      canSpawn(StepInfo& info) const noexcept override;

      void
      preSpawn(StepInfo& info, EntityShPtr ent) override;

    private:

      bool
      depleted() const noexcept;

    private:

      /**
       * @brief - The number of mobs to spawn. This value is set
       *          to `-1` in case an infinite amount of mobs can
       *          be spawned.
       */
      int m_reserve;

      /**
       * @brief - The amount of mobs already spawned by this
       *          element. Increases indefinitely in case the
       *          spawner can always spawn mobs.
       */
      int m_spawned;

      /**
       * @brief - The interval to consider spawning a new mob.
       *          It is expressed in milliseconds and attempts
       *          will only occur when an interval expires.
       */
      Duration m_interval;

      /**
       * @brief - The timestamp at which a mob was generated
       *          last by this spawner.
       */
      TimeStamp m_last;

      /**
       * @brief - Holds the remaining duration before a spawn
       *          is allowed for this spawner in case a pause
       *          event occurs. Note that in case the spawner
       *          is not allowed to spawn anything anymore it
       *          won't contain anything.
       */
      Duration m_passed;
  };

  using TimedSpawnerShPtr = std::shared_ptr<TimedSpawner>;
}

# include "TimedSpawner.hxx"

#endif    /* TIMED_SPAWNER_HH */

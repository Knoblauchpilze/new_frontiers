#ifndef    TIMED_SPAWNER_HH
# define   TIMED_SPAWNER_HH

# include <memory>
# include "Spawner.hh"

namespace new_frontiers {

  class TimedSpawner: public Spawner {
    public:

      /**
       * @brief - Create a new portal that is able to spawn new
       *          entities on a countdown. The amount of entities
       *          that can be spawned can be limited if needed.
       * @param tile - the visual representation of the spawner
       *               along with its position.
       * @param interval - the time interval between consecutive
       *                   spawns of entities.
       * @param mob - the type of mob spawned by this element.
       * @param id - the variant of mob spawned.
       * @param stock - a value indicating how many entities can
       *                be spawned by this portal. The default
       *                value of `-1` indicate that an infinite
       *                amount of entities can be spawned.
       */
      TimedSpawner(const BlockTile& tile,
                   const Duration& interval,
                   const tiles::Entity& mob,
                   int id = 0,
                   int stock = -1);

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

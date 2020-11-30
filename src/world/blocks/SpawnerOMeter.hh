#ifndef    SPAWNER_OMETER_HH
# define   SPAWNER_OMETER_HH

# include <memory>
# include "Spawner.hh"

namespace new_frontiers {

  class SpawnerOMeter: public Spawner {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a spawner. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct SOMProps: SProps {
        float threshold;
        float reserve;
        float refill;
      };

      /**
       * @brief - Create a new portal that is able to spawn new
       *          entities based on the amount of resources that
       *          is stored in it. Each spawn consumes certain
       *          quantity of resource and there's an additional
       *          interval that can be set to avoid spawning all
       *          entities at once.
       * @param props - the list of props to create the spawner.
       */
      SpawnerOMeter(const SOMProps& props);

      /**
       * @brief - Return a floating point value measuring the
       *          progress made in the spawn of a new entity.
       *          It does not take the time into account but
       *          only the stock of resource compared to the
       *          threshold required to spawn a mob.
       * @return - a value in the range `[0; 1]` measuring a
       *           progress towards spawning a new mob.
       */
      float
      getCompletion() const noexcept;

      /**
       * @brief - Used to change the amount of the resource that
       *          is managed by this spawner by the specified
       *          value. It can be either used to refill this
       *          portal or to deplete even more the stock.
       *          In case the amount is too large to be removed
       *          from the internal stock, the operation is not
       *          performed unless the `force` boolean says so.
       *          We just guarantee that the stock does not go
       *          below `0`.
       * @param delta - the amount of resource to add or deplete.
       * @param force - `true` if the refill operation should
       *                always be successful.
       * @return - the value that was actually added/removed in
       *           the deposit.
       */
      float
      refill(float delta, bool force = false);

    protected:

      /**
       * @brief - Implementation of the interface method to
       *          handle the automatic refill of the stock.
       * @param info - information to perform the update.
       */
      void
      update(StepInfo& info) override;

      /**
       * @brief - Used to define the refill rate for this spawner.
       *          The rate is not checked to be positive which is
       *          allowing spawner that are actually emptying on
       *          their own over time.
       * @param rate - the new refill rate.
       */
      void
      setRefillRate(float rate) noexcept;

      bool
      canSpawn(StepInfo& info) const noexcept override;

      void
      preSpawn(StepInfo& info, EntityShPtr ent) override;

    private:

      /**
       * @brief - The current stock of resource available in the
       *          spawner. Based on the cost of a single entity
       *          it can be enough to spawn a new one.
       */
      float m_stock;

      /**
       * @brief - The cost of a single entity. This amount is
       *          removed from the `m_stock` each time a new
       *          entity is spawned.
       */
      float m_threshold;

      /**
       * @brief - Base refill rate for this spawner. This value
       *          is added to the `m_stock` assuming it is the
       *          amount of stock that is refilled every second.
       */
      float m_refill;
  };

  using SpawnerOMeterShPtr = std::shared_ptr<SpawnerOMeter>;
}

# include "SpawnerOMeter.hxx"

#endif    /* SPAWNER_OMETER_HH */

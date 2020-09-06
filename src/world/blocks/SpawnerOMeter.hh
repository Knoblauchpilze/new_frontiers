#ifndef    SPAWNER_OMETER_HH
# define   SPAWNER_OMETER_HH

# include <memory>
# include "TimedSpawner.hh"

namespace new_frontiers {

  class SpawnerOMeter: public TimedSpawner {
    public:

      /**
       * @brief - Create a new portal that is able to spawn new
       *          entities based on the amount of resources that
       *          is stored in it. Each spawn consumes certain
       *          quantity of resource and there's an additional
       *          interval that can be set to avoid spawning all
       *          entities at once.
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
      SpawnerOMeter(const BlockTile& tile,
                    float threshold,
                    float stock,
                    const Duration& interval,
                    const tiles::Entity& mob,
                    int id = 0);

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
       * @return - `true` if the operation could be performed.
       */
      bool
      refill(float delta, bool force = false);

    protected:

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
      int m_stock;

      /**
       * @brief - The cost of a single entity. This amount is
       *          removed from the `m_stock` each time a new
       *          entity is spawned.
       */
      int m_threshold;
  };

  using SpawnerOMeterShPtr = std::shared_ptr<SpawnerOMeter>;
}

# include "SpawnerOMeter.hxx"

#endif    /* SPAWNER_OMETER_HH */

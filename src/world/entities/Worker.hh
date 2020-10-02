#ifndef    WORKER_HH
# define   WORKER_HH

# include "Mob.hh"

namespace new_frontiers {

  class Worker: public Mob {
    public:

      /**
       * @brief - Creates a new mob with the specified tile
       *          and a default VFX to emit.
       * @param tile - the visual representation of the entity
       *               along with its position.
       */
      Worker(const EntityTile& tile);

    protected:

      bool
      collect(StepInfo& info, float& x, float& y) override;

      bool
      getBack(StepInfo& info, float& x, float& y) override;

      bool
      wander(StepInfo& info, float& x, float& y) override;

    private:
  };

  using WorkerShPtr = std::shared_ptr<Worker>;
}

# include "Worker.hxx"

#endif    /* WORKER_HH */

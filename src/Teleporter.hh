#ifndef    TELEPORTER_HH
# define   TELEPORTER_HH

# include <memory>
# include "SolidElement.hh"

namespace new_frontiers {

  class Teleporter: public SolidElement {
    public:

      /**
       * @brief - Build a new portal with its associated visual
       *          display.
       * @param tile - the visual representation of the portal
       *               along with its position.
       */
      Teleporter(const SolidTile& tile);

      /**
       * @brief - Implementation of the interface method to step
       *          the teleporter ahead in time. Such an object
       *          transports an entity from a position to another
       *          after a certain period of time.
       * @param info - the needed information about the world.
       * @return - `true` if an entity has been spawned.
       */
      // bool
      // step(StepInfo& info) override;

  };

  using TeleporterShPtr = std::shared_ptr<Teleporter>;
}

# include "Teleporter.hxx"

#endif    /* TELEPORTER_HH */

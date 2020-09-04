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

  };

  using TeleporterShPtr = std::shared_ptr<Teleporter>;
}

# include "Teleporter.hxx"

#endif    /* TELEPORTER_HH */

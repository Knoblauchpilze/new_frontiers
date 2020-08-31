#ifndef    TELEPORTER_HXX
# define   TELEPORTER_HXX

# include "Teleporter.hh"

namespace new_frontiers {

  inline
  Teleporter::Teleporter(const SolidTile& tile):
    SolidElement(tile, "portal")
  {}

}

#endif    /* TELEPORTER_HXX */

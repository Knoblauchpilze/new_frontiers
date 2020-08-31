#ifndef    WORLD_TYPES_HXX
# define   WORLD_TYPES_HXX

# include "WorldTypes.hh"

namespace new_frontiers {

  template <typename TileType>
  Tile<TileType>
  newTile(const TileType& type, int id) noexcept {
    Tile<TileType> t;

    t.x = 0.0f;
    t.y = 0.0f;

    t.type = type;
    t.id = id;

    return t;
  }

  inline
  void
  StepInfo::clampCoord(float& x, float& y) const noexcept {
    x = std::min(std::max(x, xMin), xMax);
    y = std::min(std::max(y, yMin), yMax);
  }

}

#endif    /* WORLD_TYPES_HXX */

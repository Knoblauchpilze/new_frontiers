#ifndef    WORLD_TYPES_HXX
# define   WORLD_TYPES_HXX

# include "WorldTypes.hh"

namespace new_frontiers {

  template <typename TileType>
  Tile<TileType>
  newTile(const TileType& type, int id) noexcept {
    Tile<TileType> t;

    t.x = 0;
    t.y = 0;

    t.type = type;
    t.id = id;

    return t;
  }

  /**
   * @brief - Represents a solid element.
   */
  using SolidTile = Tile<Sprite>;

  /**
   * @brief - Represents an entity.
   */
  using EntityTile = Tile<Mob>;

  /**
   * @brief - Represents a visual effect.
   */
  using VFXTile = Tile<Effect>;

}

#endif    /* WORLD_TYPES_HXX */

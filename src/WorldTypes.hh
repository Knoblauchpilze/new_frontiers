#ifndef    WORLD_TYPES_HH
# define   WORLD_TYPES_HH

namespace new_frontiers {

  /**
   * @brief - Enumeration of possible tile types.
   */
  enum Sprite {
    Empty,
    Wall_Dirt,
    Wall_Stone,
    Wall_Catacomb,
    Wall_Slime,
    Well,
    Statue,
    Fluid,
    Portal,

    SpritesCount //<- Used to make sure that we know how many
                 //<- sprites we're handling.
  };

  /**
   * @brief - Used for convenience to reference to
   *          an entity type in the game.
   */
  enum Mob {
    Knight,
    Snake,
    Warlord,
    Vampire,
    Princess,
    Sphynx,

    MobsCount
  };

  /**
   * @brief - References the explosion and various effects
   *          that are visible in the game.
   */
  enum Effect {
    Fire,
    Electricity,
    Gas,
    Smoke,

    EffectsCount
  };

  /**
   * @brief - Convenience structure which defines the element of
   *          a tile in the world along with the status and some
   *          useful info about the tile.
   *          We can define several type of tiles based on the
   *          actual content: typical use cases include solid
   *          elements, entities and effects.
   * @param type - the type of the tile, defining the main role
   *               of the tile.
   */
  template <typename TileType>
  struct Tile {
    int x;
    int y;
    TileType type;
  };

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

#endif    /* WORLD_TYPES_HH */

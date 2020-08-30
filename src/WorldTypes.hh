#ifndef    WORLD_TYPES_HH
# define   WORLD_TYPES_HH

namespace new_frontiers {

  /**
   * @brief - Enumeration of possible tile types.
   */
  enum Sprite {
    Empty,
    Wall,
    Door,
    Portal,

    SpritesCount //<- Used to make sure that we know how many
                 //<- sprites we're handling.
  };

  /**
   * @brief - Used for convenience to reference to
   *          an entity type in the game.
   */
  enum Mob {
    MarineKnight,
    Warrior,
    StoneGiant,
    Sorceress,
    Knight,
    GeneticExperiment,
    Warlord,
    Vampire,
    Gecko,
    DarkAnubis,
    Cyclope,
    Fool,
    Beast,
    BlueAvenger,
    SoulEater,
    Valkyrie,
    Guardian,
    Blob,
    CosmicThreat,
    Gorgone,
    DemonBat,
    Griffin,
    Executioner,
    MindlessGolem,
    IncaOverlord,
    Hydra,
    Dragon,
    Sorcerer,
    Satyr,
    TwoHeadedWarrior,
    FireKnight,

    MobsCount
  };

  /**
   * @brief - References the explosion and various effects
   *          that are visible in the game.
   */
  enum Effect {
    Fire,
    Lightning,
    Poison,
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
    float x;
    float y;
    TileType type;
    int id;
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

  /**
   * @brief - Used to create a new tile with the specified type
   *          and identifier. The coordinates are set to `0`.
   * @param type - the type of the tile.
   * @param id - the index of the variation of the tile type.
   * @return - the created tile.
   */
  template <typename TileType>
  Tile<TileType>
  newTile(const TileType& type, int id = 0) noexcept;

}

# include "WorldTypes.hxx"

#endif    /* WORLD_TYPES_HH */

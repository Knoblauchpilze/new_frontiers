#ifndef    TILES_HH
# define   TILES_HH

# include <string>

namespace new_frontiers {

  namespace tiles {

    /**
     * @brief - Enumeration of possible block types.
     */
    enum Block {
      Empty,
      Wall,
      Door,
      Portal,

      BlocksCount //<- Used to make sure that we know how many
                  //<- block we're handling.
    };

    /**
     * @brief - Used for convenience to reference to
     *          an entity type in the game.
     */
    enum Entity {
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

      EntitiesCount
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

  }

  /**
   * @brief - Used to convert a string into a valid
   *          block value. In case the string does
   *          not correspond to any known block a
   *          `BlocksCount` value is returned.
   * @param str - the string to convert.
   * @return - the corresponding block.
   */
  tiles::Block
  strToBlock(const std::string& str) noexcept;

  /**
   * @brief - Used to convert a string into a valid
   *          entity value. In case the string does
   *          not correspond to any known sprite a
   *          value of `EntitiesCount` is is returned.
   * @param str - the string to convert.
   * @return - the corresponding entity.
   */
  tiles::Entity
  strToEntity(const std::string& str) noexcept;

  /**
   * @brief - Used to convert a string into a valid
   *          VFX value. In case the string does not
   *          correspond to any known sprite a value
   *          of `EffectsCount` is is returned.
   * @param str - the string to convert.
   * @return - the corresponding VFX.
   */
  tiles::Effect
  strToEffect(const std::string& str) noexcept;

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
   * @brief - Represents a block.
   */
  using BlockTile = Tile<tiles::Block>;

  /**
   * @brief - Represents an entity.
   */
  using EntityTile = Tile<tiles::Entity>;

  /**
   * @brief - Represents a visual effect.
   */
  using VFXTile = Tile<tiles::Effect>;

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

# include "Tiles.hxx"

#endif    /* TILES_HH */

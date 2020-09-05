#ifndef    TILES_HXX
# define   TILES_HXX

# include "Tiles.hh"

namespace new_frontiers {

  inline
  Sprite
  strToSprite(const std::string& str) noexcept {
    if (str == "Empty") {
      return Empty;
    }
    if (str == "Wall") {
      return Wall;
    }
    if (str == "Door") {
      return Door;
    }
    if (str == "Portal") {
      return Portal;
    }

    return SpritesCount;
  }

  inline
  Mob
  strToMob(const std::string& str) noexcept {
    if (str == "MarineKnight") {
      return MarineKnight;
    }
    if (str == "Warrior") {
      return Warrior;
    }
    if (str == "StoneGiant") {
      return StoneGiant;
    }
    if (str == "Sorceress") {
      return Sorceress;
    }
    if (str == "Knight") {
      return Knight;
    }
    if (str == "GeneticExperiment") {
      return GeneticExperiment;
    }
    if (str == "Warlord") {
      return Warlord;
    }
    if (str == "Vampire") {
      return Vampire;
    }
    if (str == "Gecko") {
      return Gecko;
    }
    if (str == "DarkAnubis") {
      return DarkAnubis;
    }
    if (str == "Cyclope") {
      return Cyclope;
    }
    if (str == "Fool") {
      return Fool;
    }
    if (str == "Beast") {
      return Beast;
    }
    if (str == "BlueAvenger") {
      return BlueAvenger;
    }
    if (str == "SoulEater") {
      return SoulEater;
    }
    if (str == "Valkyrie") {
      return Valkyrie;
    }
    if (str == "Guardian") {
      return Guardian;
    }
    if (str == "Blob") {
      return Blob;
    }
    if (str == "CosmicThreat") {
      return CosmicThreat;
    }
    if (str == "Gorgone") {
      return Gorgone;
    }
    if (str == "DemonBat") {
      return DemonBat;
    }
    if (str == "Griffin") {
      return Griffin;
    }
    if (str == "Executioner") {
      return Executioner;
    }
    if (str == "MindlessGolem") {
      return MindlessGolem;
    }
    if (str == "IncaOverlord") {
      return IncaOverlord;
    }
    if (str == "Hydra") {
      return Hydra;
    }
    if (str == "Dragon") {
      return Dragon;
    }
    if (str == "Sorcerer") {
      return Sorcerer;
    }
    if (str == "Satyr") {
      return Satyr;
    }
    if (str == "TwoHeadedWarrior") {
      return TwoHeadedWarrior;
    }
    if (str == "FireKnight") {
      return FireKnight;
    }

    return MobsCount;
  }

  inline
  Effect
  strToEffect(const std::string& str) noexcept {
    if (str == "Fire") {
      return Fire;
    }
    if (str == "Lightning") {
      return Lightning;
    }
    if (str == "Poison") {
      return Poison;
    }
    if (str == "Smoke") {
      return Smoke;
    }

    return EffectsCount;
  }

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

}

#endif    /* TILES_HXX */

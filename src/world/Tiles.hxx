#ifndef    TILES_HXX
# define   TILES_HXX

# include "Tiles.hh"

namespace new_frontiers {

  inline
  tiles::Block
  strToBlock(const std::string& str) noexcept {
    if (str == "Empty") {
      return tiles::Empty;
    }
    if (str == "Wall") {
      return tiles::Wall;
    }
    if (str == "Door") {
      return tiles::Door;
    }
    if (str == "Portal") {
      return tiles::Portal;
    }

    return tiles::BlocksCount;
  }

  inline
  tiles::Entity
  strToEntity(const std::string& str) noexcept {
    if (str == "MarineKnight") {
      return tiles::MarineKnight;
    }
    if (str == "Warrior") {
      return tiles::Warrior;
    }
    if (str == "StoneGiant") {
      return tiles::StoneGiant;
    }
    if (str == "Sorceress") {
      return tiles::Sorceress;
    }
    if (str == "Knight") {
      return tiles::Knight;
    }
    if (str == "GeneticExperiment") {
      return tiles::GeneticExperiment;
    }
    if (str == "Warlord") {
      return tiles::Warlord;
    }
    if (str == "Vampire") {
      return tiles::Vampire;
    }
    if (str == "Gecko") {
      return tiles::Gecko;
    }
    if (str == "DarkAnubis") {
      return tiles::DarkAnubis;
    }
    if (str == "Cyclope") {
      return tiles::Cyclope;
    }
    if (str == "Fool") {
      return tiles::Fool;
    }
    if (str == "Beast") {
      return tiles::Beast;
    }
    if (str == "BlueAvenger") {
      return tiles::BlueAvenger;
    }
    if (str == "SoulEater") {
      return tiles::SoulEater;
    }
    if (str == "Valkyrie") {
      return tiles::Valkyrie;
    }
    if (str == "Guardian") {
      return tiles::Guardian;
    }
    if (str == "Blob") {
      return tiles::Blob;
    }
    if (str == "CosmicThreat") {
      return tiles::CosmicThreat;
    }
    if (str == "Gorgone") {
      return tiles::Gorgone;
    }
    if (str == "DemonBat") {
      return tiles::DemonBat;
    }
    if (str == "Griffin") {
      return tiles::Griffin;
    }
    if (str == "Executioner") {
      return tiles::Executioner;
    }
    if (str == "MindlessGolem") {
      return tiles::MindlessGolem;
    }
    if (str == "IncaOverlord") {
      return tiles::IncaOverlord;
    }
    if (str == "Hydra") {
      return tiles::Hydra;
    }
    if (str == "Dragon") {
      return tiles::Dragon;
    }
    if (str == "Sorcerer") {
      return tiles::Sorcerer;
    }
    if (str == "Satyr") {
      return tiles::Satyr;
    }
    if (str == "TwoHeadedWarrior") {
      return tiles::TwoHeadedWarrior;
    }
    if (str == "FireKnight") {
      return tiles::FireKnight;
    }

    return tiles::EntitiesCount;
  }

  inline
  tiles::Effect
  strToEffect(const std::string& str) noexcept {
    if (str == "Fire") {
      return tiles::Fire;
    }
    if (str == "Lightning") {
      return tiles::Lightning;
    }
    if (str == "Poison") {
      return tiles::Poison;
    }
    if (str == "Smoke") {
      return tiles::Smoke;
    }

    return tiles::EffectsCount;
  }

  template <typename TileType>
  Tile<TileType>
  newTile(const TileType& type, int id) noexcept {
    Tile<TileType> t;

    t.p.x = 0.0f;
    t.p.y = 0.0f;

    t.type = type;
    t.id = id;

    return t;
  }

}

#endif    /* TILES_HXX */

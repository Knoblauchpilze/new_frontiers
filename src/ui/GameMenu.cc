
# include "GameMenu.hh"

namespace new_frontiers {

  void
  GameMenu::initialize() {
    // Define the background.
    BackgroundDesc bg = newTiledBackground(
      "data/img/menu/dark_basalt.png",
      olc::vi2d(200, 150)
    );

    setBackground(bg);

    // Register the minimap.
    int mbw = 150;
    olc::vi2d mbSize(mbw, m_size.y);
    MenuShPtr minimap = std::make_shared<Menu>(
      m_pos, mbSize,
      newTiledBackground(
        "data/img/menu/minimap.png",
        olc::vi2d(mbw, m_size.y)
      ),
      "minimap"
    );

    addMenu(minimap);

    // Register building categories.
    olc::vi2d bDims(40, 40);
    MenuShPtr creature = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5, 5),
      bDims,
      newTiledBackground(
        "data/img/menu/devil_mask.png",
        bDims
      ),
      "creature_menu"
    );

    addMenu(creature);

    MenuShPtr housing = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5, 5 + bDims.x + 5),
      bDims,
      newTiledBackground(
        "data/img/menu/house.png",
        bDims
      ),
      "housing_menu"
    );

    addMenu(housing);

    MenuShPtr spells = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5, 5 + 2 * (bDims.x + 5)),
      bDims,
      newTiledBackground(
        "data/img/menu/spells.png",
        bDims
      ),
      "spells_menu"
    );

    addMenu(spells);

    // Add creature menu.
    MenuShPtr griffin = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5 + bDims.x + 10, 5),
      bDims,
      newTiledBackground(
        "data/img/menu/griffin.png",
        bDims
      ),
      "griffin_menu"
    );

    addMenu(griffin);

    MenuShPtr dragon = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5 + bDims.x + 10 + (bDims.x + 5), 5),
      bDims,
      newTiledBackground(
        "data/img/menu/dragon.png",
        bDims
      ),
      "dragon_menu"
    );

    addMenu(dragon);

    MenuShPtr golem = std::make_shared<Menu>(
      m_pos + olc::vi2d(mbw + 5 + bDims.x + 10 + 2 * (bDims.x + 5), 5),
      bDims,
      newTiledBackground(
        "data/img/menu/golem.png",
        bDims
      ),
      "golem_menu"
    );

    addMenu(golem);

  }

}

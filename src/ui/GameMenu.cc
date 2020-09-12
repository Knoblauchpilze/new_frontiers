
# include "GameMenu.hh"

namespace new_frontiers {

  GameMenu::GameMenu(const olc::vi2d& pos,
                     const olc::vf2d& size):
    Menu(pos, size, "menu")
  {
    initialize();
  }

  void
  GameMenu::initialize() {
    // Define the background.
    BackgroundDesc bg = newTiledBackground(
      "data/img/menu/dark_basalt.png",
      olc::vi2d(200, 150)
    );
    setBackground(bg);

    MenuContentDesc mcd = newTextContent("");
    setContent(mcd);


    // Register the minimap.
    // TODO: Restore this.
    // int mbw = 150;
    // olc::vi2d mbSize(mbw, 0);
    // MenuShPtr minimap = std::make_shared<Menu>(
    //   m_pos, mbSize,
    //   newTiledBackground(
    //     "data/img/menu/minimap.png",
    //     olc::vi2d(mbw, 0)
    //   ),
    //   "minimap"
    // );

    // addMenu(minimap);

    // Register building categories.
    olc::vi2d bDims(40, 40);
    MenuShPtr creature = std::make_shared<Menu>(olc::vi2d(), bDims, "creature_menu");
    // creature->setBackground(newColoredBackground(olc::Pixel(59, 71, 17)));
    creature->setBackground(newColoredBackground(olc::Pixel(0, 0, 0)));
    creature->setContent(newImageContent("data/img/menu/devil_mask.png", bDims));
    addMenu(creature);

    MenuShPtr housing = std::make_shared<Menu>(olc::vi2d(0, 5 + bDims.y), bDims, "housing_menu");
    housing->setBackground(newColoredBackground(olc::Pixel(30, 66, 75)));
    housing->setContent(newImageContent("data/img/menu/house.png", bDims));
    addMenu(housing);

    MenuShPtr spells = std::make_shared<Menu>(olc::vi2d(0, 5 + 2 * bDims.y), bDims, "housing_menu");
    spells->setBackground(newColoredBackground(olc::Pixel(30, 66, 42)));
    spells->setContent(newImageContent("data/img/menu/spells.png", bDims));
    addMenu(spells);

    // Add creature menu.
    // MenuShPtr griffin = std::make_shared<Menu>(
    //   m_pos + olc::vi2d(mbw + 5 + bDims.x + 10, 5),
    //   bDims,
    //   newTiledBackground(
    //     "data/img/menu/griffin.png",
    //     bDims
    //   ),
    //   "griffin_menu"
    // );

    // addMenu(griffin);

    // MenuShPtr dragon = std::make_shared<Menu>(
    //   m_pos + olc::vi2d(mbw + 5 + bDims.x + 10 + (bDims.x + 5), 5),
    //   bDims,
    //   newTiledBackground(
    //     "data/img/menu/dragon.png",
    //     bDims
    //   ),
    //   "dragon_menu"
    // );

    // addMenu(dragon);

    // MenuShPtr golem = std::make_shared<Menu>(
    //   m_pos + olc::vi2d(mbw + 5 + bDims.x + 10 + 2 * (bDims.x + 5), 5),
    //   bDims,
    //   newTiledBackground(
    //     "data/img/menu/golem.png",
    //     bDims
    //   ),
    //   "golem_menu"
    // );

    // addMenu(golem);

  }

}

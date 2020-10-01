
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
  }

}


# include "GameMenu.hh"

namespace new_frontiers {

  GameMenu::GameMenu(const olc::vi2d& pos,
                     const olc::vf2d& size):
    Menu(pos, size, "menu", Layout::Horizontal)
  {
    initialize();
  }

  void
  GameMenu::initialize() {
    // Define the background for this menu.
    BackgroundDesc bg = newTiledBackground("data/img/menu/dark_basalt.png", olc::vi2d(200, 150));
    setBackground(bg);
    MenuContentDesc mcd = newTextContent("");
    setContent(mcd);

    // Add each option as a child menu.
    static const int margin = 2;
    static const int wSize = 100;
    olc::vf2d tSize = getSize();
    olc::vf2d cDims(wSize, tSize.y - 2 * margin);
    olc::vi2d cPos(margin, margin);

    MenuShPtr deposits = std::make_shared<Menu>(cPos, cDims, "deposits");
    deposits->setBackground(newColoredBackground(olc::Pixel(255, 128, 0)));
    mcd = newTextContent("Deposit");
    mcd.color = olc::BLACK;
    mcd.expand = false;
    deposits->setContent(mcd);
    addMenu(deposits);

    cPos.x = margin + cDims.x + margin;
    MenuShPtr pheromons = std::make_shared<Menu>(cPos, cDims, "pheromons");
    pheromons->setBackground(newColoredBackground(olc::Pixel(255, 0, 128)));
    mcd = newTextContent("Pheromon");
    mcd.color = olc::BLACK;
    mcd.expand = false;
    pheromons->setContent(mcd);
    addMenu(pheromons);
  }

}

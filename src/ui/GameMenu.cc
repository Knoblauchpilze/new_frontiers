
# include "GameMenu.hh"

namespace new_frontiers {

  void
  GameMenu::initialize() {
    // Define the background.
    BackgroundDesc bg = newTiledBackground(
      "data/img/dark_basalt.png",
      olc::vi2d(200, 150)
    );

    setBackground(bg);
  }

}

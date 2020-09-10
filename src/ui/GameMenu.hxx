#ifndef    GAME_MENU_HXX
# define   GAME_MENU_HXX

# include "GameMenu.hh"

namespace new_frontiers {

  inline
  GameMenu::GameMenu(const olc::vi2d& pos,
                     const olc::vf2d& size):
    Menu(pos, size, newColoredBackground(olc::WHITE), "menu")
  {
    initialize();
  }

}

#endif    /* GAME_MENU_HXX */

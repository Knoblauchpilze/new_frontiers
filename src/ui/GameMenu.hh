#ifndef    GAME_MENU_HH
# define   GAME_MENU_HH

# include <memory>
# include "Menu.hh"

namespace new_frontiers {

  class GameMenu: public Menu {
    public:

      /**
       * @brief - Create a new game menu for the new frontiers app
       *          with the specified dimensions.
       * @param pos - the position of the menu in the parent app.
       * @param size - the dimensions of the menu.
       */
      GameMenu(const olc::vi2d& pos,
               const olc::vf2d& size);

      /**
       * @brief - Desctruction of the object.
       */
      ~GameMenu() = default;

    private:

      void
      initialize();
  };

  using GameMenuShPtr = std::shared_ptr<GameMenu>;
}

# include "GameMenu.hxx"

#endif    /* GAME_MENU_HH */

#ifndef    MENU_HH
# define   MENU_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "../olcPixelGameEngine.h"

namespace new_frontiers {

  class Menu: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new menu with the specified dimensions.
       * @param pos - the position of the menu in the parent app.
       * @param size - the dimensions of the menu.
       * @param name - the name of the menu (for logging purposes).
       */
      Menu(const olc::vi2d& pos,
           const olc::vf2d& size,
           const std::string& name);

      /**
       * @brief - Desctruction of the object.
       */
      ~Menu();

      /**
       * @brief- Interface method allowing to render a menu in
       *         a parent application. This is used to offload
       *         some of the rendering code from the main app
       *         and hide the internal complexity of the menu.
       *         Note: we draw on the active layer so it has
       *         to be configured before calling this method.
       * @param pge - the rendering engine to display the menu.
       */
      void
      render(olc::PixelGameEngine* pge) const;

    protected:

      /**
       * @brief - Interface method allowing inheriting classes
       *          to perform their own drawing routines on top
       *          of the base representation of the menu.
       *          This default implementation does nothing.
       * @param pge - the rendering engine to display the menu.
       */
      virtual
      void
      renderSelf(olc::PixelGameEngine* pge) const;

    private:

      /**
       * @brief - The position of the menu in screen coordinates. It
       *          corresponds to the position of the top left corner.
       */
      olc::vf2d m_pos;

      /**
       * @brief - The size of the menu in pixels.
       */
      olc::vi2d m_size;
  };

  using MenuShPtr = std::shared_ptr<Menu>;
}

# include "Menu.hxx"

#endif    /* MENU_HH */

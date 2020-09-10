#ifndef    MENU_HH
# define   MENU_HH

# include <memory>
# include <vector>
# include <core_utils/CoreObject.hh>
# include "../olcPixelGameEngine.h"
# include "BackgroundDesc.hh"

namespace new_frontiers {

  // Forward declaration to be able to use shared pointers on
  // menu elements right away.
  class Menu;
  using MenuShPtr = std::shared_ptr<Menu>;

  class Menu: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new menu with the specified dimensions.
       * @param pos - the position of the menu in the parent app.
       * @param size - the dimensions of the menu.
       * @param bg - description of the background which can either
       *             be a uniform background or a tiled sprite.
       * @param name - the name of the menu (for logging purposes).
       */
      Menu(const olc::vi2d& pos,
           const olc::vf2d& size,
           const BackgroundDesc& bg,
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

      /**
       * @brief - Adds the input menu as a child of this one.
       * @param child - the child menu to register.
       */
      void
      addMenu(MenuShPtr child);

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

      /**
       * @brief - Replace the existing background with the new
       *          one.
       * @param bg - the new background to assign.
       */
      void
      setBackground(const BackgroundDesc& bg);

    private:

      /**
       * @brief - Clear any loaded resource for this menu. Used
       *          when the visual appearance needs to be adjusted.
       */
      void
      clear();

      /**
       * @brief - Used to perform the loading of the BG tile
       *          if needed from the background description.
       */
      void
      loadBGTile();

    protected:

      /**
       * @brief - The position of the menu in screen coordinates. It
       *          corresponds to the position of the top left corner.
       */
      olc::vf2d m_pos;

      /**
       * @brief - The size of the menu in pixels.
       */
      olc::vi2d m_size;

      /**
       * @brief - The background associated to this menu. Can either
       *          represent a uniform color or a tiled image.
       */
      BackgroundDesc m_bg;

      /**
       * @brief - Hold the sprite used to tile the background of this
       *          menu if any is defined in the `m_bg` element.
       */
      olc::Decal* m_bgSprite;

      /**
       * @brief - The list of children menu for this element. Children
       *          may or may not be active and are always repainted on
       *          the parent.
       */
      std::vector<MenuShPtr> m_children;
  };

}

# include "Menu.hxx"

#endif    /* MENU_HH */

#ifndef    MENU_HH
# define   MENU_HH

# include <memory>
# include <vector>
# include <core_utils/CoreObject.hh>
# include "../olcPixelGameEngine.h"
# include "BackgroundDesc.hh"
# include "MenuContentDesc.hh"
# include "Controls.hh"

namespace new_frontiers {

  // Forward declaration to be able to use shared pointers on
  // menu elements right away.
  class Menu;
  using MenuShPtr = std::shared_ptr<Menu>;

  /**
   * @brief - Define a direction for a menu, or in general a
   *          layout for children items.
   */
  enum class Layout {
    Horizontal,
    Vertical
  };

  class Menu: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new menu with the specified dimensions.
       * @param pos - the position of the menu in the parent app.
       * @param size - the dimensions of the menu.
       * @param name - the name of the menu (for logging purposes).
       * @param layout - the layout for this menu: will define how
       *                 children are represented in the menu.
       * @param parent - the parent menu for this element. Specify
       *                 `null` in case the menu is the root of the
       *                 subsystem.
       */
      Menu(const olc::vi2d& pos,
           const olc::vf2d& size,
           const std::string& name,
           const Layout& layout = Layout::Vertical,
           Menu* parent = nullptr);

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
       * @brief - Used to process the user input defined in
       *          the argument and update the internal state
       *          of this menu if needed.
       * @param c - the controls and user input for this
       *            frame.
       * @return - `true` in case this event was processed
       *           (which means that it was relevant for the
       *           menu) and `false` otherwise. Defines some
       *           sort of causal chain and allow parent to
       *           check whether one of their children could
       *           use the events.
       */
      bool
      processUserInput(const Controls& c);

      /**
       * @brief - Adds the input menu as a child of this one.
       * @param child - the child menu to register.
       */
      void
      addMenu(MenuShPtr child);

      /**
       * @brief - Assign a new position and size to this menu.
       *          Note that the position is relative to its
       *          parent (if any).
       * @param pos - the position of this menu within its
       *              parent.
       * @param size - the size of the menu in pixels.
       */
      void
      setArea(const olc::vi2d& pos, const olc::vi2d& size) noexcept;

      /**
       * @brief - Assign a new color for the background for
       *          this menu. It replaces any information on
       *          a tile that was specified before.
       * @param c - the new background color to set.
       */
      void
      setColor(const olc::Pixel& c) noexcept;

      /**
       * @brief - Replace the existing background with the new
       *          one.
       * @param bg - the new background to assign.
       */
      void
      setBackground(const BackgroundDesc& bg);

      /**
       * @brief - Replace the existing content with the new one.
       * @param mcd - the new content description for this menu.
       */
      void
      setContent(const MenuContentDesc& mcd);

      /**
       * @brief - Retrieve the size for this menu.
       * @return - the size of this menu in pixels.
       */
      olc::vf2d
      getSize() const noexcept;

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
       * @brief - Used to obtain the absolute position of the
       *          menu within the app, considering the position
       *          of the parent (if defined).
       * @return - the absolute position of this menu.
       */
      olc::vi2d
      absolutePosition() const noexcept;

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

      /**
       * @brief - Clear any loaded resource for the content of this
       *          menu. Used when the visual appearance needs to be
       *          adjusted.
       */
      void
      clearContent();

      /**
       * @brief - Used to perform the loaded of needed tiles for
       *          the content of this menu.
       */
      void
      loadContentTile();

      /**
       * @brief - Used to adapt the size of the children menus as
       *          required by the actual size of this menu.
       */
      void
      updateChildren();

    private:

      /**
       * @brief - Convenience structure describing the current state
       *          for this menu, such as visibility and highlight.
       */
      struct State {
        bool visible;
        bool highlighted;
      };

      /**
       * @brief - Describe the current state for this menu. It is
       *          used as a way to regroup all information needed
       *          to repaint this component.
       */
      State m_state;

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
       * @brief - Describe the properties associated to the content
       *          of this menu.
       */
      MenuContentDesc m_content;

      /**
       * @brief - Hold the sprite used as an icon for this menu. It
       *          might be `null` in case none is used in the menu's
       *          content.
       */
      olc::Decal* m_mcSprite;

      /**
       * @brief - The layout for this menu. Allow to define how the
       *          children will be displayed in this menu.
       */
      Layout m_layout;

      /**
       * @brief - The parent menu for this element. Can be `null` in
       *          case the menu is the root of the subsystem.
       */
      Menu* m_parent;

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

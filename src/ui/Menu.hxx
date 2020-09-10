#ifndef    MENU_HXX
# define   MENU_HXX

# include "Menu.hh"

namespace new_frontiers {

  inline
  Menu::Menu(const olc::vi2d& pos,
             const olc::vf2d& size,
             const BackgroundDesc& bg,
             const std::string& name):
    utils::CoreObject(name),

    m_pos(pos),
    m_size(size),

    m_bg(bg),
    m_bgSprite(nullptr)
  {
    loadBGTile();
  }

  inline
  Menu::~Menu() {
    clear();
  }

  inline
  void
  Menu::renderSelf(olc::PixelGameEngine* /*pge*/) const {}

  inline
  void
  Menu::setBackground(const BackgroundDesc& bg) {
    clear();
    m_bg = bg;
    loadBGTile();
  }

  inline
  void
  Menu::clear() {
    if (m_bgSprite != nullptr) {
      delete m_bgSprite;
    }

    m_bgSprite = nullptr;
  }

}

#endif    /* MENU_HXX */

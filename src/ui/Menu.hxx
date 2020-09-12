#ifndef    MENU_HXX
# define   MENU_HXX

# include "Menu.hh"

namespace new_frontiers {

  inline
  Menu::~Menu() {
    clearContent();
    clear();
  }

  inline
  void
  Menu::setArea(const olc::vi2d& pos, const olc::vi2d& size) noexcept {
    m_pos = pos;
    m_size = size;

    // Update chidlren.
    updateChildren();
  }

  inline
  void
  Menu::setColor(const olc::Pixel& c) noexcept {
    clear();
    m_bg.color = c;
    m_bg.sprite = "";
    // Load the background for convenience in case
    // some other processes are triggered.
    loadBGTile();
  }

  inline
  void
  Menu::setBackground(const BackgroundDesc& bg) {
    clear();
    m_bg = bg;
    loadBGTile();
  }

  inline
  void
  Menu::setContent(const MenuContentDesc& mcd) {
    clearContent();
    m_content = mcd;
    loadContentTile();
  }

  inline
  olc::vi2d
  Menu::absolutePosition() const noexcept {
    olc::vi2d p(0, 0);

    if (m_parent != nullptr) {
      p = m_parent->absolutePosition();
    }

    p += m_pos;

    return p;
  }

  inline
  void
  Menu::clear() {
    if (m_bgSprite != nullptr) {
      delete m_bgSprite;
    }

    m_bgSprite = nullptr;
  }

  inline
  void
  Menu::clearContent() {
    if (m_mcSprite != nullptr) {
      delete m_mcSprite;
    }

    m_mcSprite = nullptr;
  }

}

#endif    /* MENU_HXX */

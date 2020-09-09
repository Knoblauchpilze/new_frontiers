#ifndef    MENU_HXX
# define   MENU_HXX

# include "Menu.hh"

namespace new_frontiers {

  inline
  Menu::Menu(const olc::vi2d& pos,
             const olc::vf2d& size,
             const std::string& name):
    utils::CoreObject(name),

    m_pos(pos),
    m_size(size)
  {}

  inline
  Menu::~Menu() {}

  inline
  void
  Menu::render(olc::PixelGameEngine* pge) const {
    pge->FillRectDecal(m_pos, m_size);
  }

  inline
  void
  Menu::renderSelf(olc::PixelGameEngine* /*pge*/) const {}

}

#endif    /* MENU_HXX */

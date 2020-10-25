#ifndef    VFX_MENU_HXX
# define   VFX_MENU_HXX

# include "VFXMenu.hh"
# include "VFXAction.hh"

namespace new_frontiers {

  VFXMenu::VFXMenu(const olc::vi2d& pos,
                   const olc::vf2d& size,
                   const std::string& name,
                   const action::Type& type,
                   const tiles::Effect& vfx,
                   int id,
                   Menu* parent):
    Menu(pos, size, name, Layout::Horizontal, parent),

    m_vfx(vfx),
    m_vVariant(id),
    m_type(type)
  {
    // Check the action's type.
    if (m_type != action::Type::Creation && m_type != action::Type::Deletion) {
      error(
        "Unable to create vfx menu",
        "Invalid action type " + std::to_string(static_cast<int>(m_type)) +
        " but expected " + std::to_string(static_cast<int>(action::Type::Creation)) + " or " +
        std::to_string(static_cast<int>(action::Type::Deletion))
      );
    }
  }

  inline
  VFXMenu::~VFXMenu() {}

  inline
  void
  VFXMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Push a new action with the corresponding vfx
    // and type to be executed.
    log("Pushing vfx action");
    actions.push_back(std::make_shared<VFXAction>(m_type, m_vfx, m_vVariant));
  }

}

#endif    /* VFX_MENU_HXX */

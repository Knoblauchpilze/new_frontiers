#ifndef    ENTITY_MENU_HXX
# define   ENTITY_MENU_HXX

# include "EntityMenu.hh"
# include "EntityAction.hh"

namespace new_frontiers {

  EntityMenu::EntityMenu(const olc::vi2d& pos,
                         const olc::vf2d& size,
                         const std::string& name,
                         const action::Type& type,
                         const tiles::Entity& ent,
                         int id,
                         Menu* parent):
    Menu(pos, size, name, Layout::Horizontal, parent),

    m_entity(ent),
    m_eVariant(id),
    m_type(type)
  {
    // Check the action's type.
    if (m_type != action::Type::Creation && m_type != action::Type::Deletion) {
      error(
        "Unable to create entity menu",
        "Invalid action type " + std::to_string(static_cast<int>(m_type)) +
        " but expected " + std::to_string(static_cast<int>(action::Type::Creation)) + " or " +
        std::to_string(static_cast<int>(action::Type::Deletion))
      );
    }
  }

  inline
  EntityMenu::~EntityMenu() {}

  inline
  void
  EntityMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Push a new action with the corresponding entity
    // and type to be executed.
    actions.push_back(std::make_shared<EntityAction>(m_type, m_entity, m_eVariant));
  }

}

#endif    /* ENTITY_MENU_HXX */

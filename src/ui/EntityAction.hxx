#ifndef    ENTITY_ACTION_HXX
# define   ENTITY_ACTION_HXX

# include "EntityAction.hh"

namespace new_frontiers {

  inline
  EntityAction::EntityAction(const action::Type& type,
                             const tiles::Entity& ent,
                             int variant):
    Action(type, "entity_action"),

    m_entity(ent),
    m_eVariant(variant)
  {}

  inline
  EntityAction::~EntityAction() {}

  inline
  void
  EntityAction::apply(World& /*w*/) const {
    // TODO: Implement this.
    log("Should perform entity action", utils::Level::Error);
  }

}

#endif    /* ENTITY_ACTION_HXX */

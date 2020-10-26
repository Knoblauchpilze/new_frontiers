#ifndef    ENTITY_ACTION_HXX
# define   ENTITY_ACTION_HXX

# include "EntityAction.hh"

namespace new_frontiers {

  inline
  EntityAction::EntityAction(const action::Type& type,
                             EntityPropsShPtr ent):
    Action(type, "entity_action"),

    m_entity(ent)
  {}

  inline
  EntityAction::~EntityAction() {}

  inline
  void
  EntityAction::apply(World& w) const {
    w.setEntityProps(m_entity);
  }

}

#endif    /* ENTITY_ACTION_HXX */

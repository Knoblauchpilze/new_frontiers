#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"

namespace new_frontiers {

  inline
  Entity::Entity(const EntityTile& desc,
                 const Effect& vfx):
    WorldElement(desc, "entity"),

    m_vfx(vfx)
  {}

  inline
  bool
  Entity::step(StepInfo& /*info*/) {
    return false;
  }

}

#endif    /* ENTITY_HXX */

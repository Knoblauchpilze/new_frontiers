#ifndef    VFX_ACTION_HXX
# define   VFX_ACTION_HXX

# include "VFXAction.hh"
# include "../world/effects/PheromonFactory.hh"

namespace new_frontiers {

  inline
  VFXAction::VFXAction(const action::Type& type,
                       const tiles::Effect& vfx,
                       int variant):
    Action(type, "vfx_action"),

    m_vfx(vfx),
    m_vVariant(variant)
  {}

  inline
  VFXAction::~VFXAction() {}

  inline
  void
  VFXAction::apply(World& w) const {
    // Create the properties based on the internal vfx
    // description and register that as the next action
    // for the world.
    Pheromon::PProps pp = PheromonFactory::newPheromonProps(0.0f, 0.0f, m_vfx);
    pp.tile.id = m_vVariant;

    pp.radius = 0.5f;
    pp.type = pheromon::Type::Collect;

    w.setPheromonProps(pp);
  }

}

#endif    /* VFX_ACTION_HXX */

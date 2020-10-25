#ifndef    VFX_ACTION_HXX
# define   VFX_ACTION_HXX

# include "VFXAction.hh"

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
  VFXAction::apply(World& /*w*/) const {
    // TODO: Implement this.
    log("Should perform vfx action", utils::Level::Error);
  }

}

#endif    /* VFX_ACTION_HXX */

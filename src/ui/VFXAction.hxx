#ifndef    VFX_ACTION_HXX
# define   VFX_ACTION_HXX

# include "VFXAction.hh"
# include "../world/effects/PheromonFactory.hh"

namespace new_frontiers {

  inline
  VFXAction::VFXAction(const action::Type& type,
                       VFXPropsShPtr vfx):
    Action(type, "vfx_action"),

    m_vfx(vfx)
  {}

  inline
  VFXAction::~VFXAction() {}

  inline
  void
  VFXAction::apply(World& w) const {
    w.setVFXProps(m_vfx);
  }

}

#endif    /* VFX_ACTION_HXX */

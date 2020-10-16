
# include "VFX.hh"

namespace new_frontiers {

  VFX::VFX(const Props& props):
    // The health of a VFX is more like a percentage of
    // active component left.
    Element(props.tile, props.radius, 1.0f, "vfx"),

    m_amount(props.amount)
  {}

}

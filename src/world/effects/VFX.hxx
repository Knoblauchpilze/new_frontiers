#ifndef    VFX_HXX
# define   VFX_HXX

# include "VFX.hh"

namespace new_frontiers {

  inline
  VFX::VFX(const VFXTile& tile, const std::string& name):
    // The health of a VFX is more like a percentage of
    // active component left.
    Element(tile, 1.0f, name),

    m_amount(1.0f)
  {}

  inline
  float
  VFX::getAmount() const noexcept {
    return m_amount;
  }

  inline
  bool
  VFX::step(StepInfo& info) {
    // Check whether the vfx should decay in its
    // next form.
    if (isTerminated(info.moment)) {
      return true;
    }
    
    // Make this effect progress in time.
    update(info);

    return false;
  }

  inline
  void
  VFX::pause(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  VFX::resume(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  VFX::changeAmount(float delta) noexcept {
    m_amount += delta;
    m_amount = std::min(std::max(m_amount, 0.0f), 1.0f);
  }

}

#endif    /* VFX_HXX */

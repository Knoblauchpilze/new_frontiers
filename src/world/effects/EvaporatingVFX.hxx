#ifndef    EVAPORATING_VFX_HXX
# define   EVAPORATING_VFX_HXX

# include "EvaporatingVFX.hh"

namespace new_frontiers {

  inline
  EvaporatingVFX::EvaporatingVFX(const VFXTile& tile,
                                 float radius,
                                 float evaporation):
    VFX(tile, radius, "evaporating"),

    m_evaporation(std::max(evaporation, 0.0f))
  {}

  inline
  bool
  EvaporatingVFX::isTerminated(const TimeStamp& /*moment*/) const noexcept {
    // We consider the effect evaporated when it
    // reaches a negative value.
    return (getAmount() <= 0.0f);
  }

  inline
  void
  EvaporatingVFX::update(StepInfo& info) noexcept {
    // Decrease the remaining quantity for this effect
    // by an amount consistent with the elapsed time.
    changeAmount(-info.elapsed * m_evaporation);
  }

}

#endif    /* EVAPORATING_VFX_HXX */

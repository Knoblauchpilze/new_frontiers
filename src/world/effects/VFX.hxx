#ifndef    VFX_HXX
# define   VFX_HXX

# include "VFX.hh"

namespace new_frontiers {

  inline
  VFX::Props::~Props() {}

  inline
  float
  VFX::getAmount() const noexcept {
    return m_amount;
  }

  inline
  void
  VFX::step(StepInfo& info) {
    // Check whether the vfx should decay in its
    // next form.
    if (isTerminated(info.moment)) {
      // Mark this vfx for deletion.
      markForDeletion(true);
      info.removeVFX(this);

      return;
    }

    // Make this effect progress in time.
    update(info);

    return;
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

#ifndef    DECAYING_VFX_HXX
# define   DECAYING_VFX_HXX

# include "DecayingVFX.hh"

namespace new_frontiers {

  inline
  void
  DecayingVFX::pause(const utils::TimeStamp& t) {
    // In case we're decaying, we need to pause the
    // process. The metric to determine whether we
    // are decaying is based on whether the `m_phase`
    // is in the future.
    m_decayTimeLeft = utils::Duration::zero();

    if (m_transition < m_phases.size()) {
      m_decayTimeLeft = m_next - t;
    }
  }

  inline
  void
  DecayingVFX::resume(const utils::TimeStamp& t) {
    // In case the decay time left is not `0`, we
    // have to restore it.
    if (m_decayTimeLeft != utils::Duration::zero()) {
      m_next = t + m_decayTimeLeft;
    }
  }

  inline
  bool
  DecayingVFX::isTerminated(const utils::TimeStamp& /*moment*/) const noexcept {
    // If we reached the last transition, it's time
    // for this effect to disappear.
    return (m_transition >= m_phases.size());
  }

}

#endif    /* DECAYING_VFX_HXX */

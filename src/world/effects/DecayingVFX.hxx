#ifndef    DECAYING_VFX_HXX
# define   DECAYING_VFX_HXX

# include "DecayingVFX.hh"

namespace new_frontiers {

  inline
  DecayingVFX::DecayingVFX(const VFXTile& tile,
                           float radius,
                           const std::vector<Duration>& phases):
    VFX(tile, radius, "decaying"),

    m_phases(phases),
    m_transition(0u),

    m_next(Duration::zero()),
    m_decayTimeLeft()
  {
    // Adjust the next phase duration if any transitions
    // is specified.
    if (!m_phases.empty()) {
      m_transition = 0u;
      m_next = now() + m_phases[m_transition];
    }
  }

  inline
  void
  DecayingVFX::pause(const TimeStamp& t) {
    // In case we're decaying, we need to pause the
    // process. The metric to determine whether we
    // are decaying is based on whether the `m_phase`
    // is in the future.
    m_decayTimeLeft = Duration::zero();

    if (m_transition < m_phases.size()) {
      m_decayTimeLeft = m_next - t;
    }
  }

  inline
  void
  DecayingVFX::resume(const TimeStamp& t) {
    // In case the decay time left is not `0`, we
    // have to restore it.
    if (m_decayTimeLeft != Duration::zero()) {
      m_next = t + m_decayTimeLeft;
    }
  }

  inline
  bool
  DecayingVFX::isTerminated(const TimeStamp& /*moment*/) const noexcept {
    // If we reached the last transition, it's time
    // for this effect to disappear.
    return (m_transition >= m_phases.size());
  }

}

#endif    /* DECAYING_VFX_HXX */

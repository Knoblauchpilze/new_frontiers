#ifndef    VFX_HXX
# define   VFX_HXX

# include "VFX.hh"

namespace new_frontiers {

  inline
  VFX::VFX(const VFXTile& tile, bool decaying):
    Element(tile, "vfx"),

    m_decaying(decaying),

    m_decay(toMilliseconds(250)),
    m_lastDecay(decaying ? toMilliseconds(500) : toMilliseconds(1500)),

    m_transitions(decaying ? 2 : 0),

    m_phase(),
    m_decayTimeLeft()
  {
    if (m_decaying) {
      m_phase = now() + m_decay;
    }
    else {
      m_phase = now() + m_lastDecay;
    }
  }

  inline
  void
  VFX::pause(const TimeStamp& t) {
    // In case we're decaying, we need to pause the
    // process. The metric to determine whether we
    // are decaying is based on whether the `m_phase`
    // is in the future.
    m_decayTimeLeft = toMilliseconds(0);

    if (m_phase > t) {
      m_decayTimeLeft = m_phase - t;
    }
  }

  inline
  void
  VFX::resume(const TimeStamp& t) {
    // In case the decay time left is not `0`, we
    // have to restore it.
    if (m_decayTimeLeft != Milliseconds::zero()) {
      m_phase = t + m_decayTimeLeft;
    }
  }

}

#endif    /* VFX_HXX */

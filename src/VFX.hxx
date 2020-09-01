#ifndef    VFX_HXX
# define   VFX_HXX

# include "VFX.hh"

namespace new_frontiers {

  inline
  VFX::VFX(const VFXTile& tile, bool decaying):
    WorldElement(tile, "vfx"),

    m_decaying(decaying),

    m_decay(toMilliseconds(250)),
    m_lastDecay(decaying ? toMilliseconds(500) : toMilliseconds(1500)),

    m_transitions(decaying ? 2 : 0),

    m_phase()
  {
    if (m_decaying) {
      m_phase = now() + m_decay;
    }
    else {
      m_phase = now() + m_lastDecay;
    }
  }

  inline
  bool
  VFX::step(StepInfo& info) {
    // Check whether the vfx should decay in its
    // next form.
    if (info.moment < m_phase) {
      return false;
    }

    // If we reached the last transition, it's
    // time for this effect to disappear.
    if (m_transitions <= 0) {
      return true;
    }

    // Otherwise, move to the next phase for
    // this vfx and reset the decay time.
    ++m_tile.id;
    --m_transitions;

    if (m_transitions > 0) {
      m_phase = info.moment + m_decay;
    }
    else {
      m_phase = info.moment + m_lastDecay;
    }

    return false;
  }

}

#endif    /* VFX_HXX */

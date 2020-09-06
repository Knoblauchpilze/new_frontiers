
# include "VFX.hh"

namespace new_frontiers {

  bool
  VFX::step(StepInfo& info) {
    // Check whether the vfx should decay in its
    // next form.
    if (info.moment < m_next) {
      return false;
    }

    // If we reached the last transition, it's
    // time for this effect to disappear.
    if (m_transition >= m_phases.size()) {
      return true;
    }

    // Otherwise, move to the next phase for
    // this vfx and reset the decay time.
    ++m_tile.id;
    ++m_transition;

    m_next = info.moment;
    if (m_transition < m_phases.size()) {
      m_next = info.moment + m_phases[m_transition];
    }

    return false;
  }

}


# include "DecayingVFX.hh"

namespace new_frontiers {

  DecayingVFX::DecayingVFX(const DProps& props):
    VFX(props),

    m_phases(props.phases),
    m_transition(0u),

    m_next(utils::Duration::zero()),
    m_decayTimeLeft()
  {
    // Adjust the next phase duration if any transitions
    // is specified.
    if (!m_phases.empty()) {
      m_transition = 0u;
      m_next = utils::now() + m_phases[m_transition];
    }
  }

  void
  DecayingVFX::update(StepInfo& info) noexcept {
    // Check whether the vfx should decay in its
    // next form.
    if (info.moment < m_next) {
      return;
    }

    // If we reached the last transition, we are
    // not able to move to the next state.
    if (m_transition >= m_phases.size()) {
      return;
    }

    // Otherwise, we can move to the next phase
    // and reset the decay time.
    ++m_tile.id;
    ++m_transition;

    m_next = info.moment;
    if (m_transition < m_phases.size()) {
      m_next = info.moment + m_phases[m_transition];
    }
  }

}

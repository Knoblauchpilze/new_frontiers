#ifndef    PLAYER_HXX
# define   PLAYER_HXX

# include "Player.hh"

namespace new_frontiers {

  inline
  void
  Player::pause(const TimeStamp& t) {
    // In case the player has an effect applied, save
    // the remaining duration for later use.
    m_remaining = Duration::zero();

    if (m_state.glowing || m_state.exhausted) {
      m_remaining = t - m_origin;
    }
  }

  inline
  void
  Player::resume(const TimeStamp& t) {
    // Restore the remaining state if any was saved.
    if (m_remaining != Duration::zero()) {
      m_origin = t + m_remaining;
    }
  }

  inline
  void
  Player::prepareForStep(const StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  bool
  Player::takeAction(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Nothing to do.
    return false;
  }

  inline
  void
  Player::postStep(StepInfo& /*info*/) {
    // Nothing to do.
  }

}

#endif    /* PLAYER_HXX */

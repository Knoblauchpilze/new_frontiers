#ifndef    PLAYER_HXX
# define   PLAYER_HXX

# include "Player.hh"

namespace new_frontiers {

  inline
  Player::Player(const EntityTile& tile):
    Entity(tile),

    m_sprintSpeed(3.0f),
    m_recoverySpeed(m_sprintSpeed / 4.0f),

    m_exhaustion(toMilliseconds(3000)),
    m_recovery(toMilliseconds(1000)),

    m_origin(),
    m_remaining()
  {
    m_speed = m_sprintSpeed / 2.0f;
  }

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
  Player::takeAction(const StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Nothing to do.
  }

}

#endif    /* PLAYER_HXX */

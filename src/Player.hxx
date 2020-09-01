#ifndef    PLAYER_HXX
# define   PLAYER_HXX

# include "Player.hh"

namespace new_frontiers {

  inline
  Player::Player(float x, float y):
    Entity(newTile(Knight, 0), Poison),

    m_sprintSpeed(3.0f),
    m_recoverySpeed(m_sprintSpeed / 4.0f),

    m_exhaustion(toMilliseconds(3000)),
    m_recovery(toMilliseconds(1000)),

    m_origin()
  {
    m_tile.x = x;
    m_tile.y = y;

    m_speed = m_sprintSpeed / 2.0f;
  }

}

#endif    /* PLAYER_HXX */

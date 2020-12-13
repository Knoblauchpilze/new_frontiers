
# include "Player.hh"
# include "PlayerControls.hh"

namespace new_frontiers {

  Player::Player(const PProps& props):
    Entity(props),

    m_sprintSpeed(props.sprintSpeed),
    m_recoverySpeed(props.recoverySpeed),

    m_exhaustion(props.exhaustion),
    m_recovery(props.recovery),

    m_origin(),
    m_remaining()
  {
    m_speed = m_sprintSpeed / 2.0f;
  }

  void
  Player::step(StepInfo& info) {
    // Check whether any of the control keys
    // are pressed and move accordingly.
    // Note that as we're displaying cells
    // in a reverse direction we need to
    // make controls move in both directions
    // to be intuitive.

    // Determine the speed of the player based
    // on the current state.
    float sp = m_speed;

    if (m_state.glowing) {
      sp = m_sprintSpeed;
    } else if (m_state.exhausted) {
      sp = m_recoverySpeed;
    }

    if (info.controls.keys[MoveRight]) {
      // Right is moving along the negative
      // `x` axis and conversely.
      m_tile.p.x() -= info.elapsed * std::sqrt(sp);
      m_tile.p.y() += info.elapsed * std::sqrt(sp);
    }

    if (info.controls.keys[MoveUp]) {
      // Up is moving along the negative
      // `y` axis and conversely.
      m_tile.p.y() -= info.elapsed * std::sqrt(sp);
      m_tile.p.x() -= info.elapsed * std::sqrt(sp);
    }

    if (info.controls.keys[MoveLeft]) {
      m_tile.p.x() += info.elapsed * std::sqrt(sp);
      m_tile.p.y() -= info.elapsed * std::sqrt(sp);
    }

    if (info.controls.keys[MoveDown]) {
      m_tile.p.y() += info.elapsed * std::sqrt(sp);
      m_tile.p.x() += info.elapsed * std::sqrt(sp);
    }

    // Update the state of the player if he's
    // currently recovering or sprinting.
    if (m_state.exhausted && m_origin + m_recovery <= info.moment) {
      m_state.exhausted = false;
      log("Player is not exhausted anymore");
    }
    if (m_state.glowing && m_origin + m_exhaustion <= info.moment) {
      m_state.glowing = false;
      m_state.exhausted = true;
      m_origin = info.moment;
      log("Player is now exhausted (and no more glowing)");
    }

    // Update the sprinting status.
    if (info.controls.keys[Sprint] && !m_state.glowing && !m_state.exhausted) {
      m_state.glowing = true;
      m_origin = info.moment;
      log("Player is now glowing");
    }
  }

}

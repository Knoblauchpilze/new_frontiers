
# include "Player.hh"

namespace new_frontiers {

  bool
  Player::step(StepInfo& info) {
    // Check whether any of the control keys
    // are pressed and move accordingly.
    // Note that as we're displaying cells
    // in a reverse direction we need to
    // make controls move in both directions
    // to be intuitive.
    bool moved = false;

    // Determine the speed of the player based
    // on the current state.
    float sp = m_speed;

    if (m_state.glowing) {
      sp = m_sprintSpeed;
    } else if (m_state.exhausted) {
      sp = m_recoverySpeed;
    }

    if (info.controls[MoveRight]) {
      // Right is moving along the negative
      // `x` axis and conversely.
      m_tile.x -= info.elapsed * std::sqrt(sp);
      m_tile.y += info.elapsed * std::sqrt(sp);
      moved = true;
    }

    if (info.controls[MoveUp]) {
      // Up is moving along the negative
      // `y` axis and conversely.
      m_tile.y -= info.elapsed * std::sqrt(sp);
      m_tile.x -= info.elapsed * std::sqrt(sp);
      moved = true;
    }

    if (info.controls[MoveLeft]) {
      m_tile.x += info.elapsed * std::sqrt(sp);
      m_tile.y -= info.elapsed * std::sqrt(sp);
      moved = true;
    }

    if (info.controls[MoveDown]) {
      m_tile.y += info.elapsed * std::sqrt(sp);
      m_tile.x += info.elapsed * std::sqrt(sp);
      moved = true;
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
    if (info.controls[Sprint] && !m_state.glowing && !m_state.exhausted) {
      m_state.glowing = true;
      m_origin = info.moment;
      log("Player is now glowing");
    }

    return moved;
  }

}

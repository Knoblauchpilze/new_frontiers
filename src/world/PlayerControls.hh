#ifndef    PLAYER_CONTROLS_HH
# define   PLAYER_CONTROLS_HH

# include "../Controls.hh"

namespace new_frontiers {

  /**
   * @brief - Enumeration describing the possible controls
   *          for the game. Each one is registered in the
   *          step info and made available for the entities
   *          to detect.
   */
  enum Motion {
    MoveRight = controls::keys::Right,
    MoveUp = controls::keys::Up,
    MoveLeft = controls::keys::Left,
    MoveDown = controls::keys::Down,
    Sprint = controls::keys::Space,

    ControlsCount
  };

}

#endif    /* PLAYER_CONTROLS_HH */

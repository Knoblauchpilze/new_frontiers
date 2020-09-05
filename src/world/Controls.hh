#ifndef    CONTROLS_HH
# define   CONTROLS_HH

namespace new_frontiers {

  /**
   * @brief - Enumeration describing the possible controls
   *          for the game. Each one is registered in the
   *          step info and made available for the entities
   *          to detect.
   */
  enum Controls {
    MoveRight,
    MoveUp,
    MoveLeft,
    MoveDown,
    Sprint,

    ControlsCount
  };

}

#endif    /* CONTROLS_HH */

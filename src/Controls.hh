#ifndef    CONTROLS_HH
# define   CONTROLS_HH

# include <vector>

namespace new_frontiers {

  /**
   * @brief - The list of keys that are trackes by the
   *          controls structure.
   */
  enum Keys {
    Right,
    Up,
    Left,
    Down,
    Space,

    KeysCount
  };

  /**
   * @brief - Describe a structure holding the controls
   *          that are relevant extracted from the keys
   *          pressed by the user and the mouse info.
   */
  struct Controls {
    int mPosX;
    int mPosY;

    std::vector<bool> keys;
  };

  /**
   * @brief - Create a new controls structure.
   * @return - the created structure.
   */
  Controls
  newControls() noexcept;

}

# include "Controls.hxx"

#endif    /* CONTROLS_HH */

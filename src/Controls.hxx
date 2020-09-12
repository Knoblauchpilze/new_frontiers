#ifndef    CONTROLS_HXX
# define   CONTROLS_HXX

namespace new_frontiers {

  inline
  Controls
  newControls() noexcept {
    Controls c;
    c.keys.resize(KeysCount, false);

    return c;
  }

}

#endif    /* CONTROLS_HXX */

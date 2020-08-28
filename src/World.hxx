#ifndef    WORLD_HXX
# define   WORLD_HXX

# include "World.hh"

namespace new_frontiers {

  inline
  unsigned
  World::w() const noexcept {
    return m_w;
  }

  inline
  unsigned
  World::h() const noexcept {
    return m_h;
  }

}

#endif    /* WORLD_HXX */

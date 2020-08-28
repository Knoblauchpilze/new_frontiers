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

  inline
  WorldIterator
  World::iterator() const noexcept {
    return WorldIterator(m_w, m_h, m_tiles, m_entities, m_vfx);
  }

}

#endif    /* WORLD_HXX */

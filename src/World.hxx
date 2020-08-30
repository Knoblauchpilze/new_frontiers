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
  WorldIteratorShPtr
  World::iterator() const noexcept {
    return m_it;
  }

  inline
  void
  World::generate() {
    // Generate portals.
    generatePortals();

    // Create an iterator from this.
    m_it = std::make_shared<WorldIterator>(m_w, m_h, m_tiles, m_entities, m_vfx);
  }

}

#endif    /* WORLD_HXX */

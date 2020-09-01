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
    // Generate elements.
    generateElements();

    // Create an iterator and a locator service from this.
    m_it = std::make_shared<WorldIterator>(m_w, m_h, m_tiles, m_entities, m_vfx);
    m_loc = std::make_shared<WorldElementLocator>(m_w, m_h, m_tiles, m_entities, m_vfx);
  }

  inline
  void
  World::loadDimensions(std::ifstream& in) {
    // Read each dimension: should be on separate lines.
    in >> m_w;
    if (in.eof()) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions")
      );
    }
    in >> m_h;

    if (m_w <= 0 || m_h <= 0) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions ") + std::to_string(m_w) +
        "x" + std::to_string(m_h)
      );
    }
  }

}

#endif    /* WORLD_HXX */

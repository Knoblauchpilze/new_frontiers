
# include "World.hh"
# include <core_utils/CoreException.hh>

namespace new_frontiers {

  World::World(unsigned width,
               unsigned height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height)
  {
    // Check dimensions.
    if (m_w == 0u || m_h == 0u) {
      error(
        std::string("Could not create world"),
        std::string("Invalid dimensions ") + std::to_string(m_w) + "x" + std::to_string(m_h)
      );
    }
  }

  World::~World() {}

}

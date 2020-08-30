
# include "World.hh"
# include <unordered_set>
# include <core_utils/CoreException.hh>

namespace new_frontiers {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_tiles(),
    m_entities(),
    m_vfx()
  {
    // Check dimensions.
    if (m_w <= 3 || m_h <= 3) {
      error(
        std::string("Could not create world"),
        std::string("Invalid dimensions ") + std::to_string(m_w) + "x" + std::to_string(m_h)
      );
    }

    setService("world");

    generate();
  }

  World::~World() {}

  void
  World::step(float /*tDelta*/) {
    /** TODO: Implement this **/
  }

  void
  World::generatePortals() {
    // Generate a single portal for now.
    static const int portals = 1;

    std::unordered_set<int> used;

    for (int id = 0 ; id < portals ; ++id) {
      int var = m_rng.rndInt(0, 21);
      SolidTile st = newTile(Portal, var);

      bool n = false;
      while (!n) {
        st.x = m_rng.rndInt(1, m_w - 2);
        st.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(st.y * m_w + st.x) == 0);
      }

      if (n) {
        used.insert(st.y * m_w + st.x);
      }

      log("Generating portal " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }
  }

}

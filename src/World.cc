
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
  World::generate() {
    // Generate walls at random for now.
    static const int count = 5;
    static const int walls = count;
    static const int doors = count;

    std::unordered_set<int> used;

    // Generate walls.
    int remaining = m_w * m_h;
    for (int id = 0 ; id < walls && id < remaining; ++id) {
      int var = m_rng.rndInt(0, 15);
      SolidTile st = newTile(Wall, var);

      bool n = false;
      int trials = 0;
      while (!n && trials < remaining) {
        st.x = m_rng.rndInt(1, m_w - 2);
        st.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(st.y * m_w + st.x) == 0);

        ++trials;
      }
      
      if (n) {
        used.insert(st.y * m_w + st.x);
      }

      log("Generating wall " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate doors.
    remaining = m_w * m_h - m_tiles.size();
    for (int id = 0 ; id < doors && id < remaining; ++id) {
      int var = m_rng.rndInt(0, 3);
      SolidTile st = newTile(Door, var);

      bool n = false;
      int trials = 0;
      while (!n && trials < m_w * m_h) {
        st.x = m_rng.rndInt(1, m_w - 2);
        st.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(st.y * m_w + st.x) == 0);

        ++trials;
      }
      
      if (n) {
        used.insert(st.y * m_w + st.x);
      }

      log("Generating door " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }
  }

}


# include "World.hh"
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

    generate();
  }

  World::~World() {}

  void
  World::step(float /*tDelta*/) {
    /** TODO: Implement this **/
  }

  void
  World::generate() {
    static const int portals = 1;
    static const int walls = 1;
    static const int lavas = 1;
    static const int entities = 1;
    static const int vfxs = 1;

    // Generate portals.
    for (int id = 0 ; id < portals ; ++id) {
      SolidTile st;

      st.x = m_rng.rndInt(1, m_w - 2);
      st.y = m_rng.rndInt(1, m_h - 2);
      st.type = Portal;

      log("Generating portal at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate walls.
    for (int id = 0 ; id < walls ; ++id) {
      SolidTile st;

      st.x = m_rng.rndInt(1, m_w - 2);
      st.y = m_rng.rndInt(1, m_h - 2);
      st.type = Wall_Slime;

      log("Generating wall at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate lava lakes.
    for (int id = 0 ; id < lavas ; ++id) {
      SolidTile st;

      st.x = m_rng.rndInt(1, m_w - 2);
      st.y = m_rng.rndInt(1, m_h - 2);
      st.type = Fluid;

      log("Generating fluid at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate entities.
    for (int id = 0 ; id < entities ; ++id) {
      EntityTile et;

      et.x = m_rng.rndInt(1, m_w - 2);
      et.y = m_rng.rndInt(1, m_h - 2);
      et.type = Knight;

      log("Generating entity at " + std::to_string(et.x) + "x" + std::to_string(et.y));

      m_entities.push_back(et);
    }

    // Generate VFX.
    for (int id = 0 ; id < vfxs ; ++id) {
      VFXTile vt;

      vt.x = m_rng.rndInt(1, m_w - 2);
      vt.y = m_rng.rndInt(1, m_h - 2);
      vt.type = Gas;

      log("Generating vfx at " + std::to_string(vt.x) + "x" + std::to_string(vt.y));

      m_vfx.push_back(vt);
    }
  }

}

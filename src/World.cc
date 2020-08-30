
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
    static const int portals = count;
    static const int entities = count;
    static const int vfx = count;

    std::unordered_set<int> used;

    // Generate walls.
    for (int id = 0 ; id < walls ; ++id) {
      int var = m_rng.rndInt(0, 15);
      SolidTile st = newTile(Wall, var);

      bool n = false;
      while (!n) {
        st.x = m_rng.rndInt(1, m_w - 2);
        st.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(st.y * m_w + st.x) == 0);
      }

      if (n) {
        used.insert(st.y * m_w + st.x);
      }

      log("Generating wall " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate doors.
    for (int id = 0 ; id < doors ; ++id) {
      int var = m_rng.rndInt(0, 3);
      SolidTile st = newTile(Door, var);

      bool n = false;
      while (!n) {
        st.x = m_rng.rndInt(1, m_w - 2);
        st.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(st.y * m_w + st.x) == 0);
      }

      if (n) {
        used.insert(st.y * m_w + st.x);
      }

      log("Generating door " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(st);
    }

    // Generate portals.
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

    // Generate entities.
    for (int id = 0 ; id < entities ; ++id) {
      int var = m_rng.rndInt(0, MobsCount - 1);
      EntityTile et = newTile((Mob)var, 0);

      bool n = false;
      while (!n) {
        et.x = m_rng.rndInt(1, m_w - 2);
        et.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(et.y * m_w + et.x) == 0);
      }

      if (n) {
        used.insert(et.y * m_w + et.x);
      }

      log("Generating entity " + std::to_string(et.type) + " at " + std::to_string(et.x) + "x" + std::to_string(et.y));

      m_entities.push_back(et);
    }

    // Generate vfx.
    for (int id = 0 ; id < vfx ; ++id) {
      int type = m_rng.rndInt(0, EffectsCount - 1);
      int var = m_rng.rndInt(0, 2);
      VFXTile et = newTile((Effect)type, var);

      bool n = false;
      while (!n) {
        et.x = m_rng.rndInt(1, m_w - 2);
        et.y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(et.y * m_w + et.x) == 0);
      }

      used.insert(et.y * m_w + et.x);

      log("Generating vfx " + std::to_string(et.type) + " with var " + std::to_string(var) + " at " + std::to_string(et.x) + "x" + std::to_string(et.y));

      m_vfx.push_back(et);
    }

  }

}

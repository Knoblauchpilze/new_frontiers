
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
    m_vfx(),

    m_it(nullptr)
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
    // Spawn entities from spawner that can do so.
    std::vector<EntityShPtr> eSpawned;
    std::vector<VFXShPtr> vfxSpawned;

    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      SolidElementShPtr se = m_tiles[id];

      se->step(eSpawned, m_rng);
    }

    // Register entities that have been spawned.
    m_entities.insert(m_entities.end(), eSpawned.cbegin(), eSpawned.cend());

    // Make entities evolve.
    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      EntityShPtr ep = m_entities[id];

      ep->step(vfxSpawned, m_rng);
    }

    // Register vfx that have been created.
    m_vfx.insert(m_vfx.end(), vfxSpawned.cbegin(), vfxSpawned.cend());

    // Make vfx evolve.
    std::vector<int> toRm;

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      VFXShPtr v = m_vfx[id];

      if (v->step(m_rng)) {
        toRm.push_back(id);
      }
    }

    // Remove dead effects. We use reverse iterators as
    // we want to traverse the vector from end to front
    // (so as to keep indices valid while removing) and
    // using a classic for loop would lead to issue if
    // we use an `unsigned` value (we could static_cast
    // but it seems less pretty).
    for (std::vector<int>::reverse_iterator it = toRm.rbegin() ;
         it != toRm.rend() ;
         ++it)
    {
      m_vfx.erase(m_vfx.begin() + toRm[*it]);
    }

    // In case something has been modified, refresh
    // the iterator on this world.
    if (!eSpawned.empty() || !vfxSpawned.empty() || !toRm.empty()) {
      m_it->refresh();
    }
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
      int x = 0;
      int y = 0;
      while (!n) {
        x = m_rng.rndInt(1, m_w - 2);
        y = m_rng.rndInt(1, m_h - 2);

        n = (used.count(y * m_w + x) == 0);
      }

      used.insert(y * m_w + x);

      st.x = x;
      st.y = y;

      log("Generating portal " + std::to_string(var) + " at " + std::to_string(st.x) + "x" + std::to_string(st.y));

      m_tiles.push_back(std::make_shared<Spawner>(st));
    }
  }

}

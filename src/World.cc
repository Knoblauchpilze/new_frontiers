
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/CoreException.hh>
# include "Spawner.hh"
# include "Teleporter.hh"

namespace new_frontiers {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_tiles(),
    m_entities(),
    m_vfx(),

    m_it(nullptr),
    m_loc(nullptr)
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
    StepInfo si{
      0.0f,
      1.0f * m_w - 1.0f,
      0.0f,
      1.0f * m_h - 1.0f,

      m_rng,

      std::vector<EntityShPtr>(),
      std::vector<VFXShPtr>(),

      now(),

      m_loc
    };

    // Spawn entities from spawner that can do so.
    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      SolidElementShPtr se = m_tiles[id];

      se->step(si);
    }

    // Register entities that have been spawned. We
    // need to make sure that they're spawned within
    // the world's boundaries.
    m_entities.insert(m_entities.end(), si.eSpawned.cbegin(), si.eSpawned.cend());

    // Make entities evolve.
    bool eMoved = false;
    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      EntityShPtr ep = m_entities[id];

      if (ep->step(si)) {
        // TODO: This might be a real issue in case
        // there are a lot of entities because we're
        // effectively asking a full re-sort on each
        // frame.
        // Maybe we could separate re-sort at the
        // very least.
        eMoved = true;
      }
    }

    // Register vfx that have been created.
    m_vfx.insert(m_vfx.end(), si.vSpawned.cbegin(), si.vSpawned.cend());

    // Make vfx evolve.
    std::vector<int> toRm;

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      VFXShPtr v = m_vfx[id];

      if (v->step(si)) {
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
    if (!si.eSpawned.empty() || eMoved || !si.vSpawned.empty() || !toRm.empty()) {
      m_it->refresh();
    }
  }

  void
  World::generatePortals() {
    // Generate mob portal.
    SolidTile st = newTile(Portal, 3);
    st.x = 3; st.y = 4;
    m_tiles.push_back(std::make_shared<Spawner>(st, DarkAnubis));

    // Generate entry portal.
    st = newTile(Portal, 6);
    st.x = 1; st.y = 1;
    m_tiles.push_back(std::make_shared<Teleporter>(st));

    // Generate exit portal.
    st = newTile(Portal, 1);
    st.x = 6; st.y = 3;
    m_tiles.push_back(std::make_shared<Teleporter>(st));
  }

}

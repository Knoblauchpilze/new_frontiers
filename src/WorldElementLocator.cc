
# include "WorldElementLocator.hxx"

namespace {

  template <typename T>
  inline
  void
  swap(T& t1, T& t2) {
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
  }

}

namespace new_frontiers {

  WorldElementLocator::WorldElementLocator(int width,
                                           int height,
                                           const std::vector<SolidElementShPtr>& tiles,
                                           const std::vector<EntityShPtr>& entities,
                                           const std::vector<VFXShPtr>& vfx):
    utils::CoreObject("locator"),

    m_w(width),
    m_h(height),

    m_tiles(tiles),
    m_entities(entities),
    m_vfx(vfx),

    m_solidIDs()
  {
    setService("world");

    initialize();
  }

  int
  WorldElementLocator::countEntities(const Mob& mob, float x, float y, float radius) const {
    int count = 0;

    // Traverse entities and search for `mob`s.
    float r2 = radius * radius;

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      const EntityTile& et = m_entities[id]->getTile();

      if (et.type == mob && d2(x, y, et.x, et.y) <= r2) {
        ++count;
      }
    }

    return count;
  }

  void
  WorldElementLocator::initialize() {
    // Register each solid tile in the map.
    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      const SolidTile& st = m_tiles[id]->getTile();
      
      m_solidIDs.insert(static_cast<int>(st.y) * m_w + static_cast<int>(st.x));
    }
  }

}

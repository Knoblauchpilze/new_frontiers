
# include "WorldIterators.hh"
# include <algorithm>

namespace new_frontiers {

  void
  WorldIterator::sort() {
    // Generate the entries to sort.
    m_sortedTiles.resize(m_tiles.size());
    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      m_sortedTiles[id] = SortEntry{
        m_tiles[id]->getTile().x,
        m_tiles[id]->getTile().y,
        static_cast<int>(id)
      };
    }

    m_sortedEntities.resize(m_entities.size());
    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      m_sortedEntities[id] = SortEntry{
        m_entities[id]->getTile().x,
        m_entities[id]->getTile().y,
        static_cast<int>(id)
      };
    }

    m_sortedVFX.resize(m_vfx.size());
    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      m_sortedVFX[id] = SortEntry{
        m_vfx[id]->getTile().x,
        m_vfx[id]->getTile().y,
        static_cast<int>(id)
      };
    }

    auto cmp = [](const SortEntry& lhs, const SortEntry& rhs) {
      return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
    };

    std::sort(m_sortedTiles.begin(), m_sortedTiles.end(), cmp);
    std::sort(m_sortedEntities.begin(), m_sortedEntities.end(), cmp);
    std::sort(m_sortedVFX.begin(), m_sortedVFX.end(), cmp);
  }

}

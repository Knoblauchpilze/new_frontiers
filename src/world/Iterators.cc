
# include "Iterators.hh"
# include <algorithm>

namespace new_frontiers {

  void
  Iterator::sort() {
    // Generate the entries to sort.
    m_sortedBlocks.resize(m_blocks.size());
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_sortedBlocks[id] = SortEntry{
        m_blocks[id]->getTile().x,
        m_blocks[id]->getTile().y,
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

    std::sort(m_sortedBlocks.begin(), m_sortedBlocks.end(), cmp);
    std::sort(m_sortedEntities.begin(), m_sortedEntities.end(), cmp);
    std::sort(m_sortedVFX.begin(), m_sortedVFX.end(), cmp);
  }

}

#ifndef    ITERATORS_HXX
# define   ITERATORS_HXX

# include "Iterators.hh"

namespace new_frontiers {

  inline
  Iterator::Iterator(int width,
                     int height,
                     const std::vector<BlockShPtr>& blocks,
                     const std::vector<EntityShPtr>& entities,
                     const std::vector<VFXShPtr>& vfx):
    utils::CoreObject("iterator"),

    m_w(width),
    m_h(height),

    m_blocks(blocks),
    m_entities(entities),
    m_vfx(vfx),

    m_sortedBlocks(),
    m_sortedEntities(),
    m_sortedVFX()
  {
    setService("world");

    // Sort tiles, entities and vfx to allow correct
    // display: we want to sort by increasing `y` and
    // `x` values.
    sort();
  }

  inline
  int
  Iterator::w() const noexcept {
    return m_w;
  }

  inline
  int
  Iterator::h() const noexcept {
    return m_h;
  }

  inline
  int
  Iterator::blocksCount() const noexcept {
    return m_blocks.size();
  }

  inline
  int
  Iterator::entitiesCount() const noexcept {
    return m_entities.size();
  }

  inline
  int
  Iterator::vfxCount() const noexcept {
    return m_vfx.size();
  }

  inline
  BlockDesc
  Iterator::block(int id) const noexcept {
    BlockShPtr b = m_blocks[m_sortedBlocks[id].id];

    return BlockDesc{
      b->getTile(),
      b->getHealthRatio()
    };
  }

  inline
  EntityDesc
  Iterator::entity(int id) const noexcept {
    EntityShPtr e = m_entities[m_sortedEntities[id].id];

    return EntityDesc{
      e->getTile(),
      e->getRadius(),
      e->getHealthRatio(),
      e->getState(),
      e->getPathX(),
      e->getPathY(),
      e->m_cPoints
    };
  }

  inline
  VFXDesc
  Iterator::vfx(int id) const noexcept {
    VFXShPtr v = m_vfx[m_sortedVFX[id].id];

    return VFXDesc{
      v->getTile(),
      v->getAmount()
    };
  }

  inline
  void
  Iterator::refresh() {
    // Clear sorted elements.
    m_sortedBlocks.clear();
    m_sortedEntities.clear();
    m_sortedVFX.clear();

    // Resort elements.
    sort();
  }

}

#endif    /* ITERATORS_HXX */

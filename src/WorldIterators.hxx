#ifndef    WORLD_ITERATORS_HXX
# define   WORLD_ITERATORS_HXX

# include "WorldIterators.hh"

namespace new_frontiers {

  inline
  WorldIterator::WorldIterator(int width,
                               int height,
                               const std::vector<SolidElementShPtr>& tiles,
                               const std::vector<EntityShPtr>& entities,
                               const std::vector<VFXShPtr>& vfx):
    utils::CoreObject("iterator"),

    m_w(width),
    m_h(height),

    m_tiles(tiles),
    m_entities(entities),
    m_vfx(vfx),

    m_sortedTiles(),
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
  WorldIterator::w() const noexcept {
    return m_w;
  }

  inline
  int
  WorldIterator::h() const noexcept {
    return m_h;
  }

  inline
  int
  WorldIterator::solidTilesCount() const noexcept {
    return m_tiles.size();
  }

  inline
  int
  WorldIterator::entitiesCount() const noexcept {
    return m_entities.size();
  }

  inline
  int
  WorldIterator::vfxCount() const noexcept {
    return m_vfx.size();
  }

  inline
  const SolidTile&
  WorldIterator::solidTile(int id) const noexcept {
    return m_tiles[m_sortedTiles[id].id]->getDesc();
  }

  inline
  const EntityTile&
  WorldIterator::entity(int id) const noexcept {
    return m_entities[m_sortedEntities[id].id]->getDesc();
  }

  inline
  const VFXTile&
  WorldIterator::vfx(int id) const noexcept {
    return m_vfx[m_sortedVFX[id].id]->getDesc();
  }

  inline
  void
  WorldIterator::refresh() {
    // Clear sorted elements.
    m_sortedTiles.clear();
    m_sortedEntities.clear();
    m_sortedVFX.clear();

    // Resort elements.
    sort();
  }

}

#endif    /* WORLD_ITERATORS_HXX */

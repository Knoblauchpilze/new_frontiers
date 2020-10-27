#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"
# include <algorithm>
# include "entities/Mob.hh"
# include "blocks/SpawnerOMeter.hh"

namespace new_frontiers {

  namespace world {

    inline
    std::string
    focusToString(const colony::Priority& focus) noexcept {
      switch (focus) {
        case colony::Priority::Consolidation:
          return "consolidation";
        case colony::Priority::Expansion:
          return "expansion";
        case colony::Priority::War:
          return "war";
        default:
          return "unknown";
      }
    }

  }

  inline
  int
  Locator::w() const noexcept {
    return m_w;
  }

  inline
  int
  Locator::h() const noexcept {
    return m_h;
  }

  inline
  world::Block
  Locator::block(int id) const noexcept {
    BlockShPtr b = m_blocks[id];

    world::Block bd{
      b->getTile(),
      b->getHealthRatio(),
      -1.0f
    };

    SpawnerOMeterShPtr som = std::dynamic_pointer_cast<SpawnerOMeter>(b);
    if (som != nullptr) {
      bd.ratio = som->getCompletion();
    }

    return bd;
  }

  inline
  world::Entity
  Locator::entity(int id) const noexcept {
    EntityShPtr e = m_entities[id];

    world::Entity ed{
      e->getTile(),
      e->getRadius(),
      e->getPerceptionRadius(),
      e->getHealthRatio(),
      0.0f,
      0.0f,
      e->getState(),
      e->getPathX(),
      e->getPathY(),
      e->m_cPoints
    };

    MobShPtr m = std::dynamic_pointer_cast<Mob>(e);
    if (m != nullptr) {
      ed.cargo = m->getCarryingCapacity();
      ed.carrying = m->getCarried();
    }

    return ed;
  }

  inline
  world::VFX
  Locator::vfx(int id) const noexcept {
    VFXShPtr v = m_vfxs[id];

    return world::VFX{
      v->getTile(),
      v->getRadius(),
      v->getAmount()
    };
  }

  inline
  int
  Locator::coloniesCount() const noexcept {
    return static_cast<int>(m_colonies.size());
  }

  inline
  world::Colony
  Locator::colony(int id) const noexcept {
    ColonyShPtr c = m_colonies[id];

    return world::Colony{
      c->getOwner(),
      c->getFocus(),
      c->getActionRatio(),
      c->isActive()
    };
  }

  inline
  void
  Locator::refresh() {
    m_blocksIDs.clear();
    initialize();
  }

  inline
  bool
  Locator::obstructed(float x, float y) const noexcept {
    // Convert input coordinates to integer cell
    // coordinates.
    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);

    // Verify that no block occupies this location.
    return m_blocksIDs.count(yi * m_w + xi) > 0;
  }

  inline
  world::ItemEntry
  Locator::getClosest(float x,
                      float y,
                      const world::ItemType& type,
                      const world::Filter& filter) const noexcept
  {
    // Use the dedicated handler.
    std::vector<world::ItemEntry> all = getVisible(x, y, -1.0f, &type, &filter, world::Sort::Distance);

    // Return the closest one if any has
    // been found or an invalid entry. As
    // we requested the list to be sorted
    // we can pick the first element.
    if (all.empty()) {
      return world::ItemEntry{type, -1};
    }

    return all.front();
  }

  inline
  std::vector<BlockShPtr>
  Locator::getVisible(float x,
                      float y,
                      float r,
                      const tiles::Block* bTile,
                      int id,
                      const world::Filter* filter,
                      world::Sort sort) const noexcept
  {
    // Fetch visible blocks descriptions.
    world::ItemType t = world::ItemType::Block;
    std::vector<world::ItemEntry> bds = getVisible(x, y, r, &t, filter, sort);

    std::vector<BlockShPtr> bs;
    for (unsigned i = 0u ; i < bds.size() ; ++i) {
      BlockShPtr b = m_blocks[bds[i].index];

      // Not the same tile.
      if (bTile != nullptr && b->getTile().type != *bTile) {
        continue;
      }

      // Not the same variant.
      if (id != -1 && b->getTile().id != id) {
        continue;
      }

      // The block should be considered.
      bs.push_back(b);
    }

    return bs;
  }

  inline
  std::vector<EntityShPtr>
  Locator::getVisible(float x,
                      float y,
                      float r,
                      const tiles::Entity* eTile,
                      int id,
                      const world::Filter* filter,
                      world::Sort sort) const noexcept
  {
    // Fetch visible entities descriptions.
    world::ItemType t = world::ItemType::Entity;
    std::vector<world::ItemEntry> eds = getVisible(x, y, r, &t, filter, sort);

    std::vector<EntityShPtr> es;
    for (unsigned i = 0u ; i < eds.size() ; ++i) {
      EntityShPtr e = m_entities[eds[i].index];

      // Not the same tile.
      if (eTile != nullptr && e->getTile().type != *eTile) {
        continue;
      }

      // Not the same variant.
      if (id != -1 && e->getTile().id != id) {
        continue;
      }

      // The entity should be considered.
      es.push_back(e);
    }

    return es;
  }

  inline
  std::vector<VFXShPtr>
  Locator::getVisible(float x,
                      float y,
                      float r,
                      const tiles::Effect* vTile,
                      int id,
                      const world::Filter* filter,
                      world::Sort sort) const noexcept
  {
    // Fetch visible entities descriptions.
    world::ItemType t = world::ItemType::VFX;
    std::vector<world::ItemEntry> vds = getVisible(x, y, r, &t, filter, sort);

    std::vector<VFXShPtr> vs;
    for (unsigned i = 0u ; i < vds.size() ; ++i) {
      VFXShPtr v = m_vfxs[vds[i].index];

      // Not the same tile.
      if (vTile != nullptr && v->getTile().type != *vTile) {
        continue;
      }

      // Not the same variant.
      if (id != -1 && v->getTile().id != id) {
        continue;
      }

      // The VFX should be considered.
      vs.push_back(v);
    }

    return vs;
  }

  inline
  BlockShPtr
  Locator::getClosest(float x,
                      float y,
                      const tiles::Block& bTile,
                      float r,
                      int id,
                      const world::Filter* filter) const noexcept
  {
    std::vector<BlockShPtr> bs = getVisible(x, y, r, &bTile, id, filter, world::Sort::Distance);

    if (bs.empty()) {
      return nullptr;
    }

    return bs.front();
  }

  inline
  EntityShPtr
  Locator::getClosest(float x,
                      float y,
                      const tiles::Entity& eTile,
                      float r,
                      int id,
                      const world::Filter* filter) const noexcept
  {
    std::vector<EntityShPtr> es = getVisible(x, y, r, &eTile, id, filter, world::Sort::Distance);

    if (es.empty()) {
      return nullptr;
    }

    return es.front();
  }

}

#endif    /* LOCATOR_HXX */

#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"
# include <algorithm>
# include "entities/Mob.hh"
# include "blocks/SpawnerOMeter.hh"

namespace new_frontiers {

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
  void
  Locator::refresh() {
    m_blocksIDs.clear();
    initialize();
  }

  inline
  world::ItemEntry
  Locator::getClosest(float x,
                      float y,
                      const world::ItemType& type) const noexcept
  {
    // Use the dedicated handler.
    std::vector<world::ItemEntry> all = getVisible(x, y, -1.0f, &type, true);

    // Return the closest one if any has
    // been found or an invalid entry. As
    // we requested the list to be sorted
    // we can pick the first element.
    if (all.empty()) {
      return world::ItemEntry{type, -1};
    }

    return all.front();
  }

}

#endif    /* LOCATOR_HXX */

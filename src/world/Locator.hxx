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
  BlockDesc
  Locator::block(int id) const noexcept {
    BlockShPtr b = m_blocks[m_sortedBlocks[id].id];

    BlockDesc bd{
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
  EntityDesc
  Locator::entity(int id) const noexcept {
    EntityShPtr e = m_entities[m_sortedEntities[id].id];

    EntityDesc ed{
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
  VFXDesc
  Locator::vfx(int id) const noexcept {
    VFXShPtr v = m_vfx[m_sortedVFX[id].id];

    return VFXDesc{
      v->getTile(),
      v->getRadius(),
      v->getAmount()
    };
  }

  inline
  void
  Locator::refresh(const Update& update) {
    if (update == Update::Full) {
      m_blocksIDs.clear();

      initialize();
    }
  }

  inline
  BlockShPtr
  Locator::getClosest(float x,
                      float y,
                      const tiles::Block& block,
                      int id) const noexcept
  {
    // Use the dedicated handler.
    std::vector<BlockShPtr> all = getBlocks(x, y, -1.0f, block, id, true);

    // Return the closest one.
    if (all.empty()) {
      return nullptr;
    }

    // We requested blocks to be sorted, we can
    // pick the first.
    return all.front();
  }

  inline
  EntityShPtr
  Locator::getClosest(float x,
                      float y,
                      const tiles::Entity& ent,
                      int id) const noexcept
  {
    // Use the dedicated handler.
    std::vector<EntityShPtr> all = getEntities(x, y, -1.0f, ent, id, true);

    // Return the closest one.
    if (all.empty()) {
      return nullptr;
    }

    // We requested entities to be sorted, we can
    // pick the first.
    return all.front();
  }

}

#endif    /* LOCATOR_HXX */

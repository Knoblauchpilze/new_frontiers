#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"
# include <algorithm>

namespace new_frontiers {

  inline
  void
  Locator::refresh(const Update& update) {
    // TODO: Handle this. We probably need some sort of
    // locatorEntry, where the position and the index
    // are stored for easy access and then inserted in
    // a quadtree or something allowing easy spatial
    // sorting.
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

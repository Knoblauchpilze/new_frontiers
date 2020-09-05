#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"

namespace new_frontiers {

  inline
  BlockShPtr
  BlockFactory::newPortal(const BlockTile& tile) noexcept {
    return std::shared_ptr<Block>(new Block(tile, "portal"));
  }

  inline
  BlockShPtr
  BlockFactory::newEntrance(const BlockTile& tile) noexcept {
    return newPortal(tile);
  }

  inline
  BlockShPtr
  BlockFactory::newExit(const BlockTile& tile) noexcept {
    return newPortal(tile);
  }

  inline
  BlockShPtr
  BlockFactory::newWall(const BlockTile& tile) noexcept {
    return std::shared_ptr<Block>(new Block(tile, "wall"));
  }

}

#endif    /* BLOCK_FACTORY_HXX */

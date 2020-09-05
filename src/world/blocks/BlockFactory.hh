#ifndef    BLOCK_FACTORY_HH
# define   BLOCK_FACTORY_HH

# include "Block.hh"

namespace new_frontiers {

  class BlockFactory {
    public:

      static
      BlockShPtr
      newPortal(const BlockTile& tile) noexcept;

      static
      BlockShPtr
      newEntrance(const BlockTile& tile) noexcept;

      static
      BlockShPtr
      newExit(const BlockTile& tile) noexcept;

      static
      BlockShPtr
      newWall(const BlockTile& tile) noexcept;

  };

}

# include "BlockFactory.hxx"

#endif    /* BLOCK_FACTORY_HH */

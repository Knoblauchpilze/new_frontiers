#ifndef    BLOCK_FACTORY_HH
# define   BLOCK_FACTORY_HH

# include "Block.hh"

namespace new_frontiers {

  class BlockFactory {
    public:

      static
      BlockShPtr
      newPortal(int id, float x, float y) noexcept;

      static
      BlockShPtr
      newEntrance(int id, float x, float y) noexcept;

      static
      BlockShPtr
      newExit(int id, float x, float y) noexcept;

      static
      BlockShPtr
      newWall(int id, float x, float y) noexcept;

      static
      BlockShPtr
      newTimedSpawner(int id,
                      const mob::Type& type,
                      float x,
                      float y,
                      const tiles::Entity& ent,
                      int variant = 0) noexcept;

      static
      BlockShPtr
      newSpawnerOMeter(int id,
                       const mob::Type& type,
                       float x,
                       float y,
                       const tiles::Entity& ent,
                       int variant = 0) noexcept;

      static
      BlockShPtr
      newDeposit(float x, float y, float stock = 10.0f) noexcept;

    private:

      static
      BlockTile
      newTile(const tiles::Block& b, int id, float x, float y) noexcept;

      static
      BlockShPtr
      newBlock(const tiles::Block& block, int id, float x, float y, const std::string& name) noexcept;
  };

}

# include "BlockFactory.hxx"

#endif    /* BLOCK_FACTORY_HH */

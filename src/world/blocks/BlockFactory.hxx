#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"
# include "Block.hh"
# include "SpawnerOMeter.hh"
# include "Deposit.hh"

namespace new_frontiers {

  inline
  BlockShPtr
  BlockFactory::newEntrance(float x, float y, int id) noexcept {
    return newPortal(x, y, id);
  }

  inline
  BlockShPtr
  BlockFactory::newExit(float x, float y, int id) noexcept {
    return newPortal(id, x, y);
  }

  inline
  BlockShPtr
  BlockFactory::newWall(float x, float y, int id) noexcept {
    return newBlock(newTile(tiles::Wall, id, x, y), "wall");
  }

  inline
  BlockShPtr
  BlockFactory::newTimedSpawner(int id,
                                const mob::Type& type,
                                float x,
                                float y,
                                const tiles::Entity& ent,
                                int variant) noexcept
  {
    return std::make_shared<TimedSpawner>(
      newTile(tiles::Portal, id, x, y), // spawner display
      toMilliseconds(500),              // spawn interval
      ent,                              // mob
      type,                             // agent type
      variant,                          // mob variant
      1                                 // stock
    );
  }

  inline
  BlockShPtr
  BlockFactory::newSpawnerOMeter(int id,
                                 const mob::Type& type,
                                 float x,
                                 float y,
                                 const tiles::Entity& ent,
                                 int variant) noexcept
  {
    return std::make_shared<SpawnerOMeter>(
      newTile(tiles::Portal, id, x, y), // spawner display
      10.0f,                            // threshold
      9.5f,                             // stock
      toMilliseconds(100),              // interval
      ent,                              // mob
      type,                             // agent type
      variant,                          // mob variant,
      0.5f                              // refill
    );
  }

  inline
  BlockShPtr
  BlockFactory::newDeposit(float x, float y, float stock) noexcept {
    return std::make_shared<Deposit>(newTile(tiles::Portal, 14, x, y), stock);
  }

  inline
  BlockTile
  BlockFactory::newTile(const tiles::Block& b, int id, float x, float y) noexcept {
    return BlockTile{x, y, b, id};
  }

  inline
  BlockShPtr
  BlockFactory::newBlock(const BlockTile& bt, const std::string& name) noexcept {
    return std::shared_ptr<Block>(new Block(bt, name));
  }

  inline
  BlockShPtr
  BlockFactory::newPortal(float x, float y, int id) noexcept {
    return newBlock(newTile(tiles::Portal, id, x, y), "portal");
  }

}

#endif    /* BLOCK_FACTORY_HXX */

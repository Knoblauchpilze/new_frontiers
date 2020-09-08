#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"
# include "Block.hh"
// # include "TimedSpawner.hh"
# include "SpawnerOMeter.hh"

namespace new_frontiers {

  inline
  BlockShPtr
  BlockFactory::newPortal(int id, float x, float y) noexcept {
    return newBlock(tiles::Portal, id, x, y, "portal");
  }

  inline
  BlockShPtr
  BlockFactory::newEntrance(int id, float x, float y) noexcept {
    return newPortal(id, x, y);
  }

  inline
  BlockShPtr
  BlockFactory::newExit(int id, float x, float y) noexcept {
    return newPortal(id, x, y);
  }

  inline
  BlockShPtr
  BlockFactory::newWall(int id, float x, float y) noexcept {
    return newBlock(tiles::Wall, id, x, y, "wall");
  }

  inline
  BlockShPtr
  BlockFactory::newSpawner(int id, float x, float y, const tiles::Entity& ent) noexcept {
# define TIMED_SPAWNER
# ifdef TIMED_SPAWNER
    return std::make_shared<TimedSpawner>(
      newTile(tiles::Portal, id, x, y), // spawner display
      toMilliseconds(500),              // spawn interval
      ent,                              // mob
      1,                                // mob variant
      1                                 // stock
    );
# else
    return std::make_shared<SpawnerOMeter>(
      newTile(tiles::Portal, id, x, y), // spawner display
      10.0f,                            // threshold
      8.0f,                             // stock
      toMilliseconds(100),              // interval
      ent,                              // mob
      2,                                // mob variant,
      0.5f                              // refill
    );
# endif
  }

  inline
  BlockTile
  BlockFactory::newTile(const tiles::Block& b, int id, float x, float y) noexcept {
    return BlockTile{x, y, b, id};
  }

  inline
  BlockShPtr
  BlockFactory::newBlock(const tiles::Block& block, int id, float x, float y, const std::string& name) noexcept {
    return std::shared_ptr<Block>(new Block(newTile(block, id, x, y), name));
  }

}

#endif    /* BLOCK_FACTORY_HXX */

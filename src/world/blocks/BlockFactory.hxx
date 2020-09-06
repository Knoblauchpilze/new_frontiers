#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"

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
    return std::make_shared<Spawner>(newTile(tiles::Portal, id, x, y), ent);
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

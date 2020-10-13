#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"
# include "Block.hh"
# include "TimedSpawner.hh"
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
    TimedSpawner::TSProps pp;
    pp.tile = newTile(tiles::Portal, id, x, y);

    pp.radius = 1.0f;
    pp.health = 10.0f;

    pp.mob = ent;
    pp.mVariant = variant;

    pp.agent = type;

    pp.interval = toMilliseconds(500);
    pp.stock = 1;

    return std::make_shared<TimedSpawner>(pp);
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
    SpawnerOMeter::SOMProps pp;
    pp.tile = newTile(tiles::Portal, id, x, y);

    pp.radius = 1.0f;
    pp.health = 10.0f;

    pp.mob = ent;
    pp.mVariant = variant;

    pp.agent = type;

    pp.threshold = 10.0f;
    pp.reserve = 9.5f;
    pp.refill = 0.5f;

    return std::make_shared<SpawnerOMeter>(pp);
  }

  inline
  BlockShPtr
  BlockFactory::newDeposit(float x, float y, float stock) noexcept {
    Deposit::DProps pp;
    pp.tile = newTile(tiles::Portal, 14, x, y);

    pp.radius = 1.0f;
    pp.health = 10.0f;

    pp.stock = stock;

    return std::make_shared<Deposit>(pp);
  }

  inline
  BlockTile
  BlockFactory::newTile(const tiles::Block& b, int id, float x, float y) noexcept {
    return BlockTile{x, y, b, id};
  }

  inline
  BlockShPtr
  BlockFactory::newBlock(const BlockTile& bt, const std::string& name) noexcept {
    Block::Props pp;
    pp.tile = bt;

    pp.radius = 1.0f;
    pp.health = 10.0f;

    return std::shared_ptr<Block>(new Block(pp, name));
  }

  inline
  BlockShPtr
  BlockFactory::newPortal(float x, float y, int id) noexcept {
    return newBlock(newTile(tiles::Portal, id, x, y), "portal");
  }

}

#endif    /* BLOCK_FACTORY_HXX */

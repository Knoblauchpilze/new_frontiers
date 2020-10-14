#ifndef    BLOCK_FACTORY_HXX
# define   BLOCK_FACTORY_HXX

# include "BlockFactory.hh"
# include "Block.hh"
# include "TimedSpawner.hh"
# include "SpawnerOMeter.hh"
# include "Deposit.hh"

namespace new_frontiers {

  inline
  Deposit::DProps
  BlockFactory::newDepositProps(float x, float y) noexcept {
    Deposit::DProps pp;

    pp.tile = newTile(tiles::Portal, 14, x, y);

    pp.radius = sk_radius;
    pp.health = sk_health;

    pp.stock = 10.0f;

    return pp;
  }

  inline
  TimedSpawner::TSProps
  BlockFactory::newTimedSpawnerProps(float x, float y, const tiles::Entity& ent) noexcept {
    TimedSpawner::TSProps pp;

    pp.tile = newTile(tiles::Portal, 3, x, y);

    pp.radius = sk_radius;
    pp.health = sk_health;

    pp.mob = ent;
    pp.mVariant = 0;

    pp.agent = mob::Type::Worker;

    pp.spawnRadius = 2.0f;

    pp.interval = toMilliseconds(500);
    pp.stock = 1;

    return pp;
  }

  inline
  SpawnerOMeter::SOMProps
  BlockFactory::newSpawnerOMeterProps(float x, float y, const tiles::Entity& ent) noexcept {
    SpawnerOMeter::SOMProps pp;

    pp.tile = newTile(tiles::Portal, 3, x, y);

    pp.radius = sk_radius;
    pp.health = sk_health;

    pp.mob = ent;
    pp.mVariant = 0;

    pp.agent = mob::Type::Worker;

    pp.spawnRadius = 2.0f;

    pp.threshold = 10.0f;
    pp.reserve = 9.5f;
    pp.refill = 0.5f;

    return pp;
  }

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
  BlockFactory::newTimedSpawner(const TimedSpawner::TSProps& props) noexcept {
    return std::make_shared<TimedSpawner>(props);
  }

  inline
  BlockShPtr
  BlockFactory::newSpawnerOMeter(const SpawnerOMeter::SOMProps& props) noexcept {
    return std::make_shared<SpawnerOMeter>(props);
  }

  inline
  BlockShPtr
  BlockFactory::newDeposit(const Deposit::DProps& props) noexcept {
    return std::make_shared<Deposit>(props);
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

    pp.radius = sk_radius;
    pp.health = sk_health;

    return std::shared_ptr<Block>(new Block(pp, name));
  }

  inline
  BlockShPtr
  BlockFactory::newPortal(float x, float y, int id) noexcept {
    return newBlock(newTile(tiles::Portal, id, x, y), "portal");
  }

}

#endif    /* BLOCK_FACTORY_HXX */

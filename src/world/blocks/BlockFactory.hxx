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

    pp.owner.invalidate();

    return pp;
  }

  inline
  TimedSpawner::TSProps
  BlockFactory::newTimedSpawnerProps(float x, float y, const tiles::Entity& ent) noexcept {
    TimedSpawner::TSProps pp;

    pp.tile = newTile(tiles::Portal, 3, x, y);

    pp.radius = sk_radius;
    pp.health = sk_health;

    pp.owner.invalidate();

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

    pp.owner.invalidate();

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
  Block::Props
  BlockFactory::newBlockProps(float x, float y, const tiles::Block& blo) noexcept {
    Block::Props pp;

    pp.tile = newTile(blo, 0, x, y);

    pp.radius = sk_radius;
    pp.health = sk_health;

    pp.owner.invalidate();

    return pp;
  }

  inline
  Block::Props
  BlockFactory::newWallProps(float x, float y, int id) noexcept {
    Block::Props pp = newBlockProps(x, y, tiles::Wall);
    pp.tile.id = id;

    return pp;
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
  BlockShPtr
  BlockFactory::newBlock(const Block::Props& props, const std::string& name) noexcept {
    return std::shared_ptr<Block>(new Block(props, name));
  }

  inline
  BlockTile
  BlockFactory::newTile(const tiles::Block& b, int id, float x, float y) noexcept {
    return BlockTile{x, y, b, id};
  }

}

#endif    /* BLOCK_FACTORY_HXX */

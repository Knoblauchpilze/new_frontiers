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
    TimedSpawner::TSProps props;
    props.tile = newTile(tiles::Portal, id, x, y);
    props.radius = 1.0f;

    props.mob = ent;
    props.mVariant = variant;

    props.agent = type;

    props.interval = toMilliseconds(500);
    props.stock = 1;


    return std::make_shared<TimedSpawner>(props);
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
    SpawnerOMeter::SOMProps props;
    props.tile = newTile(tiles::Portal, id, x, y);
    props.radius = 1.0f;

    props.mob = ent;
    props.mVariant = variant;

    props.agent = type;

    props.interval = toMilliseconds(100);
    props.stock = -1;

    props.threshold = 10.0f;
    props.reserve = 9.5f;
    props.refill = 0.5f;

    return std::make_shared<SpawnerOMeter>(props);
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

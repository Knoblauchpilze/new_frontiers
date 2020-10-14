#ifndef    BLOCK_FACTORY_HH
# define   BLOCK_FACTORY_HH

# include "Block.hh"
# include "Deposit.hh"
# include "TimedSpawner.hh"
# include "SpawnerOMeter.hh"

namespace new_frontiers {

  class BlockFactory {
    public:

      static
      Deposit::DProps
      newDepositProps(float x, float y) noexcept;

      static
      TimedSpawner::TSProps
      newTimedSpawnerProps(float x, float y, const tiles::Entity& ent) noexcept;

      static
      SpawnerOMeter::SOMProps
      newSpawnerOMeterProps(float x, float y, const tiles::Entity& ent) noexcept;

      static
      BlockShPtr
      newEntrance(float x, float y, int id = 0) noexcept;

      static
      BlockShPtr
      newExit(float x, float y, int id = 0) noexcept;

      static
      BlockShPtr
      newWall(float x, float y, int id = 0) noexcept;

      static
      BlockShPtr
      newTimedSpawner(const TimedSpawner::TSProps& props) noexcept;

      static
      BlockShPtr
      newSpawnerOMeter(const SpawnerOMeter::SOMProps& props) noexcept;

      static
      BlockShPtr
      newDeposit(const Deposit::DProps& props) noexcept;

    private:

      static
      BlockTile
      newTile(const tiles::Block& b, int id, float x, float y) noexcept;

      static
      BlockShPtr
      newBlock(const BlockTile& bt, const std::string& name) noexcept;

      static
      BlockShPtr
      newPortal(float x, float y, int id = 0) noexcept;

    private:

      static const float sk_radius;
      static const float sk_health;
  };

}

# include "BlockFactory.hxx"

#endif    /* BLOCK_FACTORY_HH */

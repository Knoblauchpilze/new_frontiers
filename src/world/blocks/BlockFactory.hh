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
      newTimedSpawnerProps(float x, float y, const tiles::Entity& ent = tiles::Executioner) noexcept;

      static
      SpawnerOMeter::SOMProps
      newSpawnerOMeterProps(float x, float y, const tiles::Entity& ent = tiles::Executioner) noexcept;

      static
      Block::Props
      newBlockProps(float x, float y, const tiles::Block& blo) noexcept;

      static
      Block::Props
      newWallProps(float x, float y, int id = 0) noexcept;

      /**
       * @brief - Create the most specialized instance of a
       *          block given the input properties. A cast
       *          is performed to the most specialized type
       *          and then the creation of the corresponding
       *          block is triggered.
       * @param props - the properties from which the block
       *                should be created.
       * @return - the pointer to the created block or a
       *           `null` value in case the properties can't
       *           be interpreted.
       *
       */
      static
      BlockShPtr
      newBlockFromProps(const Block::Props& props) noexcept;

      static
      BlockShPtr
      newTimedSpawner(const TimedSpawner::TSProps& props) noexcept;

      static
      BlockShPtr
      newSpawnerOMeter(const SpawnerOMeter::SOMProps& props) noexcept;

      static
      BlockShPtr
      newDeposit(const Deposit::DProps& props) noexcept;

      static
      BlockShPtr
      newBlock(const Block::Props& props, const std::string& name) noexcept;

    private:

      static
      BlockTile
      newTile(const tiles::Block& b, int id, float x, float y) noexcept;

    private:

      static const float sk_radius;
      static const float sk_health;
  };

}

# include "BlockFactory.hxx"

#endif    /* BLOCK_FACTORY_HH */

#ifndef    BLOCK_HH
# define   BLOCK_HH

# include <memory>
# include "Element.hh"
# include "Tiles.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  class Block: public Element<tiles::Block> {
    public:

      /**
       * @brief - Base implementation for an element which
       *          does nothing.
       * @param info - information about the world.
       * @return - `false` as nothing happens here.
       */
      bool
      step(StepInfo& info) override;

      /**
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to pause.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to resume.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

      /**
       * @brief - Create a new solid element with the tile
       *          and name. Only used to forward the args
       *          to the base class.
       * @param tile - the visual representation of this
       *               element along with its position.
       * @param name - the name of the object.
       */
      Block(const BlockTile& tile, const std::string& name);

    private:

      friend class BlockFactory;
  };

  using BlockShPtr = std::shared_ptr<Block>;
}

# include "Block.hxx"

#endif    /* BLOCK_HH */

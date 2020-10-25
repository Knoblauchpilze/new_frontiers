#ifndef    BLOCK_ACTION_HH
# define   BLOCK_ACTION_HH

# include <memory>
# include "Action.hh"

namespace new_frontiers {

  class BlockAction: public Action {
    public:

      /**
       * @brief - Create a new action allowing to perform some
       *          modification of a block. Typical use cases
       *          include spawning or removing a block from the
       *          world.
       * @param type - the type of this action.
       * @param block - the tile of the block this action is
       *                related to.
       * @param variant - the variant of the block.
       */
      BlockAction(const action::Type& type,
                  const tiles::Block& block,
                  int variant = 0);

      /**
       * @brief - Desctruction of the object.
       */
      virtual ~BlockAction();

      /**
       * @brief - Implementation of the effect this action has
       *          on the input world. This typically results in
       *          spawning a new block or removing one.
       * @param w - the world onto which the action should be
       *            applied.
       */
      void
      apply(World& w) const override;

    private:

      /**
       * @brief - The tile of the block attached to this action.
       */
      tiles::Block m_block;

      /**
       * @brief - The variant of the block attached to this
       *          action.
       */
      int m_bVariant;
  };

  using BlockActionShPtr = std::shared_ptr<BlockAction>;
}

# include "BlockAction.hxx"

#endif    /* BLOCK_ACTION_HH */

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
       * @param block - the entity attached to this action.
       */
      BlockAction(const action::Type& type,
                  BlockPropsShPtr block);

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
       * @brief - The description of the block attached to this
       *          action.
       */
      BlockPropsShPtr m_block;
  };

  using BlockActionShPtr = std::shared_ptr<BlockAction>;
}

# include "BlockAction.hxx"

#endif    /* BLOCK_ACTION_HH */

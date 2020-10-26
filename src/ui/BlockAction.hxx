#ifndef    BLOCK_ACTION_HXX
# define   BLOCK_ACTION_HXX

# include "BlockAction.hh"
# include "../world/blocks/BlockFactory.hh"

namespace new_frontiers {

  inline
  BlockAction::BlockAction(const action::Type& type,
                           const tiles::Block& block,
                           int variant):
    Action(type, "block_action"),

    m_block(block),
    m_bVariant(variant)
  {}

  inline
  BlockAction::~BlockAction() {}

  inline
  void
  BlockAction::apply(World& w) const {
    // Create the properties based on the internal block
    // description and register that as the next action
    // for the world.
    Deposit::DProps pp = BlockFactory::newDepositProps(0.0f, 0.0f);
    pp.tile.type = m_block;
    pp.tile.id = m_bVariant;

    w.setDepositProps(pp);
  }

}

#endif    /* BLOCK_ACTION_HXX */

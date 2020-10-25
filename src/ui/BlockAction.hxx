#ifndef    BLOCK_ACTION_HXX
# define   BLOCK_ACTION_HXX

# include "BlockAction.hh"

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
  BlockAction::apply(World& /*w*/) const {
    // TODO: Implement this.
    log("Should perform block action", utils::Level::Error);
  }

}

#endif    /* BLOCK_ACTION_HXX */

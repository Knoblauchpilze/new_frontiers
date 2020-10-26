#ifndef    BLOCK_ACTION_HXX
# define   BLOCK_ACTION_HXX

# include "BlockAction.hh"
# include "../world/blocks/BlockFactory.hh"

namespace new_frontiers {

  inline
  BlockAction::BlockAction(const action::Type& type,
                           BlockPropsShPtr block):
    Action(type, "block_action"),

    m_block(block)
  {}

  inline
  BlockAction::~BlockAction() {}

  inline
  void
  BlockAction::apply(World& w) const {
    w.setBlockProps(m_block);
  }

}

#endif    /* BLOCK_ACTION_HXX */

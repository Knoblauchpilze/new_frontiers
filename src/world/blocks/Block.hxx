#ifndef    BLOCK_HXX
# define   BLOCK_HXX

# include "Block.hh"

namespace new_frontiers {

  inline
  Block::Block(const BlockTile& tile,
               const std::string& name,
               float health):
    Element(tile, health, name)
  {}

  inline
  bool
  Block::step(StepInfo& /*info*/) {
    return false;
  }

  inline
  void
  Block::pause(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Block::resume(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

}

#endif    /* BLOCK_HXX */

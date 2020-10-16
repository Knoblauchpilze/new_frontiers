#ifndef    BLOCK_HXX
# define   BLOCK_HXX

# include "Block.hh"

namespace new_frontiers {

  inline
  void
  Block::step(StepInfo& /*info*/) {
    // Nothing to do.
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

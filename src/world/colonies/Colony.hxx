#ifndef    COLONY_HXX
# define   COLONY_HXX

# include "Colony.hh"

namespace new_frontiers {

  inline
  Colony::~Colony() {
    // Nothing to do yet.
  }

  inline
  void
  Colony::setFocus(const colony::Priority& focus) {
    m_focus = focus;
  }

  inline
  void
  Colony::pause(const TimeStamp& /*t*/) {
    // TODO: Implement this.
  }

  inline
  void
  Colony::resume(const TimeStamp& /*t*/) {
    // TODO: Implement this.
  }

}

#endif    /* COLONY_HXX */

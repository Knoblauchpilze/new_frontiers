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
  Colony::step(StepInfo& info) {
    // Start by updating internal time depedent processes.
    update(info);

    // Attempt to refine the focus of the colony at the
    // moment by considering the situation.
    changeFocus(info);

    // Perform spawning of blocks if possible.
    think(info);
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

  inline
  void
  Colony::changeFocus(const StepInfo& /*info*/) {
    // TODO: Implement this.
  }

}

#endif    /* COLONY_HXX */

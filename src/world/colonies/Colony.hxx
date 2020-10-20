#ifndef    COLONY_HXX
# define   COLONY_HXX

# include "Colony.hh"
# include "Locator.hh"

namespace new_frontiers {

  inline
  Colony::~Colony() {
    // Nothing to do yet.
  }

  inline
  float
  Colony::getActionRatio() const noexcept {
    // Compute the action ratio by taking precautions
    // when manipulating possibly `0` numbers.
    return std::min(std::max(m_budget / std::max(m_actionCost, 1.0f), 0.0f), 1.0f);
  }

  inline
  colony::Priority
  Colony::getFocus() const noexcept {
    return m_focus;
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
    // Nothing to do yet.
  }

  inline
  void
  Colony::resume(const TimeStamp& /*t*/) {
    // Nothing to do yet.
  }

}

#endif    /* COLONY_HXX */

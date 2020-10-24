#ifndef    WARRIOR_HXX
# define   WARRIOR_HXX

# include "Warrior.hh"

namespace new_frontiers {

  inline
  bool
  Warrior::inhibitPheromon(StepInfo& /*info*/) const noexcept {
    return m_behavior == Behavior::Wander;
  }

}

#endif    /* WARRIOR_HXX */

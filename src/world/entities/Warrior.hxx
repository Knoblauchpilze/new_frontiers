#ifndef    WARRIOR_HXX
# define   WARRIOR_HXX

# include "Warrior.hh"

namespace new_frontiers {

  inline
  void
  Warrior::pause(const TimeStamp& t) {
    m_passed = t - m_last;
    Mob::pause(t);
  }

  inline
  void
  Warrior::resume(const TimeStamp& t) {
    m_last = t - m_passed;
    Mob::resume(t);
  }

}

#endif    /* WARRIOR_HXX */

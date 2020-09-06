#ifndef    HOSTILE_MOB_HXX
# define   HOSTILE_MOB_HXX

# include "HostileMob.hh"

namespace new_frontiers {

  inline
  HostileMob::HostileMob(const EntityTile& tile):
    Entity(tile),

    m_vfxDelay(toMilliseconds(2000)),
    m_last(now()),

    m_passed()
  {}

  inline
  void
  HostileMob::pause(const TimeStamp& t) {
    // Save the duration passed since the last
    // time a vfx was emitted by this mob.
    m_passed = t - m_last;

    // Call the pause method from the base class.
    Entity::pause(t);
  }

  inline
  void
  HostileMob::resume(const TimeStamp& t) {
    // Restore the `last` time an effect was
    // produced to be the same duration in the
    // past as when the pause occurred.
    m_last = t - m_passed;

    // Call the resume method from the base class.
    Entity::resume(t);
  }

}

#endif    /* HOSTILE_MOB_HXX */

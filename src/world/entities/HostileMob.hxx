#ifndef    HOSTILE_MOB_HXX
# define   HOSTILE_MOB_HXX

# include "HostileMob.hh"

namespace new_frontiers {

  inline
  HostileMob::HostileMob(const EntityTile& tile):
    Entity(tile),

    m_vfxDelay(toMilliseconds(2000)),
    m_last(now() - m_vfxDelay),

    m_passed(),

    m_behavior(Behavior::Wander)
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

  inline
  void
  HostileMob::prepare(StepInfo& info) {
    // Select a speed if not already done.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.1f, 3.0f);
    }
  }

  inline
  pheromon::Type
  HostileMob::behaviorToPheromon(const Behavior& b) noexcept {
    switch (b) {
      case Behavior::Chase:
        return pheromon::Type::Chase;
      case Behavior::Fight:
        return pheromon::Type::Fight;
      case Behavior::Collect:
        return pheromon::Type::Collect;
      case Behavior::Return:
        return pheromon::Type::Return;
      case Behavior::Wander:
      default:
        return pheromon::Type::Wander;
    }
  }

  inline
  bool
  HostileMob::behave(StepInfo& info) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    switch (s) {
      case Behavior::Chase:
        chase(info);
        break;
      case Behavior::Fight:
        fight(info);
        break;
      case Behavior::Collect:
        collect(info);
        break;
      case Behavior::Return:
        getBack(info);
        break;
      case Behavior::Wander:
      default:
        wander(info);
        break;
    }

    return (s != m_behavior);
  }

}

#endif    /* HOSTILE_MOB_HXX */

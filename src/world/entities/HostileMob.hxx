#ifndef    HOSTILE_MOB_HXX
# define   HOSTILE_MOB_HXX

# include "HostileMob.hh"

namespace new_frontiers {

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
  HostileMob::prepareForStep(const StepInfo& info) {
    // Select a speed if not already done.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.1f, 3.0f);
    }
  }

  inline
  void
  HostileMob::postStep(StepInfo& info) {
    // Emit a new VFX if needed.
    if (m_last + m_vfxDelay <= info.moment) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.vSpawned.push_back(spawnPheromon(pt));

      m_last = info.moment;
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
  void
  HostileMob::emitPheromon(StepInfo& info) noexcept {
    // Emit a pheromon based on the current behavior.
    pheromon::Type pt = behaviorToPheromon(m_behavior);
    info.vSpawned.push_back(spawnPheromon(pt));

    // Register this as the last moment we produced
    // a pheromon.
    m_last = info.moment;
  }

  inline
  bool
  HostileMob::behave(StepInfo& info, float& x, float&y) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    switch (s) {
      case Behavior::Chase:
        chase(info, x, y);
        break;
      case Behavior::Fight:
        fight(info, x, y);
        break;
      case Behavior::Collect:
        collect(info, x, y);
        break;
      case Behavior::Return:
        getBack(info, x, y);
        break;
      case Behavior::Wander:
      default:
        wander(info, x, y);
        break;
    }

    return (s != m_behavior);
  }

}

#endif    /* HOSTILE_MOB_HXX */

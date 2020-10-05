#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"

namespace new_frontiers {

  inline
  void
  Mob::pause(const TimeStamp& t) {
    // Save the duration passed since the last
    // time a vfx was emitted by this mob.
    m_passed = t - m_last;

    // Call the pause method from the base class.
    Entity::pause(t);
  }

  inline
  void
  Mob::resume(const TimeStamp& t) {
    // Restore the `last` time an effect was
    // produced to be the same duration in the
    // past as when the pause occurred.
    m_last = t - m_passed;

    // Call the resume method from the base class.
    Entity::resume(t);
  }

  inline
  void
  Mob::prepareForStep(const StepInfo& info) {
    // Select a speed if not already done.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.1f, 3.0f);
    }
  }

  inline
  void
  Mob::postStep(StepInfo& info) {
    // Emit a new VFX if needed.
    if (m_last + m_vfxDelay <= info.moment) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.spawnVFX(spawnPheromon(pt));

      m_last = info.moment;
    }
  }

  inline
  bool
  Mob::chase(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::fight(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::collect(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::getBack(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::wander(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Empty base implementation.
    return false;
  }

  inline
  pheromon::Type
  Mob::behaviorToPheromon(const Behavior& b) noexcept {
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
  Mob::emitPheromon(StepInfo& info) noexcept {
    // Emit a pheromon based on the current behavior.
    pheromon::Type pt = behaviorToPheromon(m_behavior);
    info.spawnVFX(spawnPheromon(pt));

    // Register this as the last moment we produced
    // a pheromon.
    m_last = info.moment;
  }

  inline
  Mob::Thought
  Mob::behave(StepInfo& info) noexcept {
    // Save the current behavior.
    Behavior s = m_behavior;

    Thought t{false, false, m_tile.x, m_tile.y};

    switch (s) {
      case Behavior::Chase:
        t.actionTaken = chase(info, t.xT, t.yT);
        break;
      case Behavior::Fight:
        t.actionTaken = fight(info, t.xT, t.yT);
        break;
      case Behavior::Collect:
        t.actionTaken = collect(info, t.xT, t.yT);
        break;
      case Behavior::Return:
        t.actionTaken = getBack(info, t.xT, t.yT);
        break;
      case Behavior::Wander:
      default:
        t.actionTaken = wander(info, t.xT, t.yT);
        break;
    }

    t.behaviorChanged = (s != m_behavior);

    return t;
  }

}

#endif    /* HOSTILE_MOB_HXX */

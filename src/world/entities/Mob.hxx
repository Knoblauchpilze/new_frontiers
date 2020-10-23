#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"

namespace new_frontiers {

  inline
  void
  Mob::PheromonInfo::accumulate(float locX, float locY, float amount) {
    x += amount * locX;
    y += amount * locY;
    w += amount;

    ++count;
  }

  inline
  void
  Mob::PheromonInfo::normalize() {
    if (count > 0) {
      x /= w;
      y /= w;

      w = 1.0f;
    }
  }

  inline
  float
  Mob::getCarryingCapacity() const noexcept {
    return m_cargo;
  }

  inline
  float
  Mob::getCarried() const noexcept {
    return m_carrying;
  }

  inline
  void
  Mob::pause(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Mob::resume(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Mob::setBehavior(const Behavior& b) noexcept {
    m_behavior = b;
  }

  inline
  float
  Mob::carry(float cargo) noexcept {
    // Compute the maximum amount still carryable
    // based on the available cargo space and the
    // actual amount to carry.
    float carryable = m_cargo - m_carrying;
    float toCarry = std::min(cargo, carryable);

    m_carrying += toCarry;

    return std::max(cargo - toCarry, 0.0f);
  }

  inline
  float
  Mob::availableCargo() const noexcept {
    return std::max(m_cargo - m_carrying, 0.0f);
  }

  inline
  void
  Mob::prepareForStep(const StepInfo& info) {
    // Select a speed if not already done.
    if (m_speed < 0.0f) {
      m_speed = info.rng.rndFloat(0.5f, 1.5f);
    }

    // Also update the energy available for
    // this frame based on the elapsed time
    // since the last update.
    m_energy += info.elapsed * m_energyRefill;
  }

  inline
  void
  Mob::postStep(StepInfo& info) {
    // Emit a new pheromon if needed.
    if (m_energy >= m_pheromonCost) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.spawnVFX(spawnPheromon(pt));

      m_energy -= m_pheromonCost;
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

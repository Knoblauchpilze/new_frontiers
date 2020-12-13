#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"

namespace new_frontiers {

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
  Mob::pause(const utils::TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Mob::resume(const utils::TimeStamp& /*t*/) {
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
      m_speed = info.rng.rndFloat(1.0f, 2.5f);
    }

    // Also update the energy available for
    // this frame based on the elapsed time
    // since the last update.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);
  }

  inline
  void
  Mob::postStep(StepInfo& info) {
    emitPheromon(info);
  }

  inline
  bool
  Mob::inhibitPheromon(StepInfo& /*info*/) const noexcept {
    return false;
  }

  inline
  bool
  Mob::chase(StepInfo& /*info*/, path::Path& /*path*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::fight(StepInfo& /*info*/, path::Path& /*path*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::collect(StepInfo& /*info*/, path::Path& /*path*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::getBack(StepInfo& /*info*/, path::Path& /*path*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::flee(StepInfo& /*info*/, path::Path& /*path*/) {
    // Empty base implementation.
    return false;
  }

  inline
  bool
  Mob::wander(StepInfo& /*info*/, path::Path& /*path*/) {
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
      case Behavior::Flee:
        return pheromon::Type::Flee;
      case Behavior::Wander:
      default:
        return pheromon::Type::Wander;
    }
  }

  inline
  void
  Mob::emitPheromon(StepInfo& info) noexcept {
    // Emit a pheromon based on the current behavior
    // if the energy is available.
    if (m_energy >= m_pheromonCost && !inhibitPheromon(info)) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.spawnVFX(spawnPheromon(pt));

      m_energy -= m_pheromonCost;
    }
  }

}

#endif    /* HOSTILE_MOB_HXX */

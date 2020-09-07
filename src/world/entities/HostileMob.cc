
# include "HostileMob.hh"

namespace new_frontiers {

  bool
  HostileMob::step(StepInfo& info) {
    // Perform the behavior of the entity.
    bool changed = behave(info);

    // Emit a new VFX if needed: that is if
    // we emitted it too long ago or if the
    // behavior just changed.
    if (changed || m_last + m_vfxDelay <= info.moment) {
      pheromon::Type pt = behaviorToPheromon(m_behavior);
      info.vSpawned.push_back(spawnPheromon(pt));

      m_last = info.moment;
    }

    // Execute the base function and use it
    // as a return value.
    return Entity::step(info);
  }

  void
  HostileMob::chase(StepInfo& info) {
    // TODO: Implement chase behavior.
  }

  void
  HostileMob::fight(StepInfo& info) {
    // TODO: Implement fight behavior.
  }

  void
  HostileMob::collect(StepInfo& info) {
    // TODO: Implement collect behavior.
  }

  void
  HostileMob::getBack(StepInfo& info) {
    // TODO: Implement return behavior.
  }

  void
  HostileMob::wander(StepInfo& info) {
    // TODO: Implement wander behavior.
  }

}

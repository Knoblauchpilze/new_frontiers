
# include "HostileMob.hh"

namespace new_frontiers {

  bool
  HostileMob::step(StepInfo& info) {
    // Emit a new VFX if needed.
    if (m_last + m_vfxDelay <= info.moment) {
      info.vSpawned.push_back(spawnVFX());

      m_last = info.moment;
    }

    // Execute the base function and use it
    // as a return value.
    return Entity::step(info);
  }

  void
  HostileMob::pause(const TimeStamp& /*t*/) {
    // TODO: Handle this.
  }

  void
  HostileMob::resume(const TimeStamp& /*t*/) {
    // TODO: Handle this.
  }

}

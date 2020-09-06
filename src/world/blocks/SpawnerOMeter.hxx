#ifndef    SPAWNER_OMETER_HXX
# define   SPAWNER_OMETER_HXX

# include "SpawnerOMeter.hh"

namespace new_frontiers {

  inline
  bool
  SpawnerOMeter::refill(float delta, bool force) {
    // In case the `delta` is positive the operation
    // always succeed.
    if (delta >= 0.0f) {
      m_stock += delta;

      return true;
    }

    // Otherwise we have to have a sufficient amount
    // of resource to perform the operation.
    if (m_stock < delta && !force) {
      return false;
    }

    m_stock = std::max(m_stock - delta, 0.0f);

    return true;
  }

  inline
  bool
  SpawnerOMeter::canSpawn(StepInfo& info) const noexcept {
    // We need to meet the conditions of the base class
    // and have enough resources.
    return (m_stock >= m_threshold) && TimedSpawner::canSpawn(info);
  }

}

#endif    /* SPAWNER_OMETER_HXX */

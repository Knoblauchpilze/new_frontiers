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
    // of resource to perform the operation. In case
    // the `force` is `true` we will decrease the
    // stock no matter if it becomes negative. If we
    // don't force the update then we only make the
    // stock `0`.
    if (m_stock < delta && force) {
      m_stock -= delta;

      return false;
    }

    m_stock = std::max(m_stock - delta, 0.0f);

    return true;
  }

  inline
  void
  SpawnerOMeter::update(StepInfo& info) {
    refill(info.elapsed * m_refill, false);
  }

  inline
  void
  SpawnerOMeter::setRefillRate(float rate) noexcept {
    m_refill = rate;
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

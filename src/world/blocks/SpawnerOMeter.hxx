#ifndef    SPAWNER_OMETER_HXX
# define   SPAWNER_OMETER_HXX

# include "SpawnerOMeter.hh"

namespace new_frontiers {

  inline
  float
  SpawnerOMeter::getCompletion() const noexcept {
    // Trivial case where there is no threshold.
    if (m_threshold <= 0.0f) {
      return m_stock < 0.0f ? 0.0f : 1.0f;
    }

    // General case.
    return std::min(std::max(m_stock / m_threshold, 0.0f), 1.0f);
  }

  inline
  float
  SpawnerOMeter::refill(float delta, bool force) {
    // In case the `delta` is positive the operation
    // always succeed.
    if (delta >= 0.0f) {
      m_stock += delta;

      return delta;
    }

    // Otherwise we have to have a sufficient amount
    // of resource to perform the operation. In case
    // the `force` is `true` we will decrease the
    // stock no matter if it becomes negative. If we
    // don't force the update then we only make the
    // stock `0`.
    if (m_stock < std::abs(delta) && force) {
      m_stock += delta;

      return delta;
    }

    float var = m_stock;
    m_stock = std::max(m_stock + delta, 0.0f);

    return var;
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
  SpawnerOMeter::canSpawn(StepInfo& /*info*/) const noexcept {
    // We need to have enough resources to spawn a
    // new mob.
    return m_stock >= m_threshold;
  }

}

#endif    /* SPAWNER_OMETER_HXX */

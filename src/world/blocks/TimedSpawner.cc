
# include "TimedSpawner.hh"

namespace new_frontiers {

  TimedSpawner::TimedSpawner(const TSProps& props):
    Spawner(props),

    // Make sure the stock is at least `1` (if it is
    // not set to an unlimited amount).
    m_reserve(props.stock < 0 ? -1 : std::max(props.stock, 1)),
    m_spawned(0),

    m_interval(props.interval),
    m_last(now() - m_interval),

    m_passed(Duration::zero())
  {}

  void
  TimedSpawner::preSpawn(StepInfo& info, EntityShPtr /*ent*/) {
    // Register this moment as the last time the
    // spawner was active.
    m_last = info.moment;

    ++m_spawned;

    // Decrease stock if available.
    if (m_reserve > 0) {
      --m_reserve;
    }
  }

}

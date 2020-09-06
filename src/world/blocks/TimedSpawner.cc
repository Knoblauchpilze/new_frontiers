
# include "TimedSpawner.hh"

namespace new_frontiers {

  TimedSpawner::TimedSpawner(const BlockTile& tile,
                             const Duration& interval,
                             const tiles::Entity& mob,
                             int id,
                             int stock):
    Spawner(tile, 2.0f, mob, id),

    // Make sure the stock is at least `1` (if it is
    // not set to an unlimited amount).
    m_reserve(stock < 0 ? -1 : std::max(stock, 1)),
    m_spawned(0),

    m_interval(interval),
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

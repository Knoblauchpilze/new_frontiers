
# include "SpawnerOMeter.hh"

namespace new_frontiers {

  SpawnerOMeter::SpawnerOMeter(const BlockTile& tile,
                               float threshold,
                               float stock,
                               const Duration& interval,
                               const tiles::Entity& mob,
                               const mob::Type& agent,
                               int id,
                               float refill):
    TimedSpawner(tile, interval, mob, agent, id),

    m_stock(stock),
    m_threshold(threshold),

    m_refill(refill)
  {}

  void
  SpawnerOMeter::preSpawn(StepInfo& info, EntityShPtr ent) {
    // Decrement the stock.
    m_stock -= m_threshold;

    // And call the base method.
    TimedSpawner::preSpawn(info, ent);
  }

}

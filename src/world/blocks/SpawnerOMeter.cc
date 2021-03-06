
# include "SpawnerOMeter.hh"

namespace new_frontiers {

  SpawnerOMeter::SpawnerOMeter(const SOMProps& props):
    Spawner(props),

    m_stock(props.reserve),
    m_threshold(props.threshold),

    m_refill(props.refill)
  {}

  void
  SpawnerOMeter::preSpawn(StepInfo& /*info*/, EntityShPtr /*ent*/) {
    // Decrement the stock.
    m_stock -= m_threshold;
  }

}

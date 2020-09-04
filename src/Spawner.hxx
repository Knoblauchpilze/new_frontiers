#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace new_frontiers {

  inline
  Spawner::Spawner(const SolidTile& tile,
                   const Mob& mob,
                   int id):
    SolidElement(tile, "spawner"),

    m_mob(mob),
    m_mobID(id),

    m_toSpawn(1),
    m_spawned(0),

    m_interval(toMilliseconds(1000)),
    m_last(now() - m_interval),

    m_radius(3.0f),
    m_threshold(1),

    m_passed()
  {}

  inline
  void
  Spawner::pause(const TimeStamp& t) {
    // Only save something if the spawner is not
    // depleted already.
    if (!depleted()) {
      m_passed = t - m_last;
    }
  }

  inline
  void
  Spawner::resume(const TimeStamp& t) {
    // In case the duration saved is not null we
    // need to restore it.
    if (m_passed != Duration::zero()) {
      m_last = t - m_passed;
    }
  }

  inline
  bool
  Spawner::depleted() const noexcept {
    return m_toSpawn == 0;
  }

  inline
  bool
  Spawner::canSpawn(const TimeStamp& moment) const noexcept {
    return moment - m_interval >= m_last;
  }

}

#endif    /* SPAWNER_HXX */

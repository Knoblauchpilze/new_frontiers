#ifndef    TIMED_SPAWNER_HXX
# define   TIMED_SPAWNER_HXX

# include "TimedSpawner.hh"

namespace new_frontiers {

  inline
  void
  TimedSpawner::pause(const TimeStamp& t) {
    // Only save something if the spawner is not
    // depleted already.
    m_passed = Duration::zero();

    if (!depleted()) {
      m_passed = t - m_last;
    }
  }

  inline
  void
  TimedSpawner::resume(const TimeStamp& t) {
    // In case the duration saved is not null we
    // need to restore it.
    if (m_passed != Duration::zero()) {
      m_last = t - m_passed;
    }
  }

  inline
  bool
  TimedSpawner::canSpawn(StepInfo& info) const noexcept {
    return !depleted() && (info.moment - m_interval >= m_last);
  }

  inline
  bool
  TimedSpawner::depleted() const noexcept {
    return (m_reserve == 0);
  }

}

#endif    /* TIMED_SPAWNER_HXX */

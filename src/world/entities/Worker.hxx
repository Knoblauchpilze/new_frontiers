#ifndef    WORKER_HXX
# define   WORKER_HXX

# include "Worker.hh"

namespace new_frontiers {

  inline
  bool
  Worker::inhibitPheromon(StepInfo& /*info*/) const noexcept {
    return m_behavior == Behavior::Wander;
  }

}

#endif    /* WORKER_HXX */

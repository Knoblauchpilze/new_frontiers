
# include "Worker.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  Worker::Worker(const EntityTile& tile):
    Mob(tile)
  {
    setService("worker");
  }

  bool
  Worker::wander(StepInfo& info, float& x, float& y) {
    // Use the base handler.
    pickRandomTarget(info, x, y);

    return true;
  }

}

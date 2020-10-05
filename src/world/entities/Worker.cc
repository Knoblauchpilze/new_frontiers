
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace new_frontiers {

  Worker::Worker(const EntityTile& tile):
    Mob(tile)
  {
    setService("worker");
  }

  bool
  Worker::collect(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement this.
    return false;
  }

  bool
  Worker::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    std::vector<BlockShPtr> deposits = info.frustum->getBlocks(x, y, m_perceptionRadius, tiles::Portal, 14);

    // In case there are no deposits, continue the
    // wandering around process.
    if (deposits.empty()) {
      pickRandomTarget(info, x, y);

      return true;
    }

    // Change to collecting behavior.
    setBehavior(Behavior::Collect);

    // Assign the target to the closest deposit.
    float b = std::numeric_limits<float>::max();
    unsigned dID = deposits.size();

    for (unsigned id = 0u ; id < deposits.size() ; ++id) {
      float dx = deposits[id]->getTile().x - m_tile.x;
      float dy = deposits[id]->getTile().y - m_tile.y;

      float d = std::sqrt(dx * dx + dy * dy);
      if (d < b) {
        dID = id;
      }
    }

    // Weird case.
    if (dID >= deposits.size()) {
      log(
        "Could not select closest deposit among " + std::to_string(deposits.size()),
        utils::Level::Error
      );

      return false;
    }

    x = deposits[dID]->getTile().x;
    y = deposits[dID]->getTile().y;

    return true;
  }

}

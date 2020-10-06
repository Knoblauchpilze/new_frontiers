
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "../blocks/Deposit.hh"

namespace new_frontiers {

  Worker::Worker(const EntityTile& tile,
                 float homeX,
                 float homeY):
    Mob(tile, homeX, homeY)
  {
    setService("worker");
  }

  bool
  Worker::collect(StepInfo& info, float& x, float& y) {
    // The collect behavior is called when the worker
    // reach a destination close enough of the source
    // and so we just need to collect the resources.
    BlockShPtr b = info.frustum->getClosest(m_tile.x, m_tile.y, tiles::Portal, 14);

    if (b == nullptr) {
      // For some reason the deposit does not exist,
      // return to wandering.
      setBehavior(Behavior::Wander);

      pickRandomTarget(info, x, y);

      return true;
    }

    DepositShPtr d = std::dynamic_pointer_cast<Deposit>(b);
    if (d == nullptr) {
      // Not able to convert to a valid deposit,
      // use wander again.
      log(
        std::string("Reached block that is not deposit in collect behavior"),
        utils::Level::Warning
      );

      setBehavior(Behavior::Wander);

      pickRandomTarget(info, x, y);

      return true;
    }

    // TODO: Handle collecting.
    log(
      "Should collect on deposit at " +
      std::to_string(d->getTile().x) + "x" + std::to_string(d->getTile().y)
    );

    // And then return to the colony.
    setBehavior(Behavior::Return);

    x = m_homeX;
    y = m_homeY;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

  bool
  Worker::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    std::vector<BlockShPtr> deposits = info.frustum->getBlocks(
      x,
      y,
      m_perceptionRadius,
      tiles::Portal,
      14,
      true
    );

    // In case there are no deposits, continue the
    // wandering around process.
    if (deposits.empty()) {
      pickRandomTarget(info, x, y);

      return true;
    }

    // Change to collecting behavior.
    setBehavior(Behavior::Collect);

    // Assign the target to the closest deposit:
    // as we requested the list to be sorted we
    // can pick the first one.
    x = deposits.front()->getTile().x;
    y = deposits.front()->getTile().y;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

}

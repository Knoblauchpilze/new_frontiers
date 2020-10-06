
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "../blocks/Deposit.hh"
# include "../blocks/SpawnerOMeter.hh"

# include <cxxabi.h>

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
        "Reached block that is not deposit in collect behavior",
        utils::Level::Warning
      );

      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    // Collect the maximum amount possible given
    // the stock of the deposit and the available
    // carrying capacity.
    float toFetch = std::min(availableCargo(), d->getStock());
    log(
      "Collecting " +
      std::to_string(toFetch) + "/" + std::to_string(d->getStock()) +
      " on deposit at " +
      std::to_string(d->getTile().x) + "x" + std::to_string(d->getTile().y)
    );

    // In case we can't fetch anything, return to
    // wander behavior.
    if (toFetch <= 0.0f) {
      log("Deposit is empty");

      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    d->refill(-toFetch);
    carry(toFetch);

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
  Worker::getBack(StepInfo& info, float& x, float& y) {
    // This method is called when the worker reaches
    // its home location. At this point we need to
    // dump the resources in the home of the entity
    // and get back to wandering.
    BlockShPtr b = info.frustum->getClosest(m_tile.x, m_tile.y, tiles::Portal, -1);

    if (b == nullptr) {
      // For some reason the home of the entity does
      // not exist, return to wandering.
      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    SpawnerOMeterShPtr s = std::dynamic_pointer_cast<SpawnerOMeter>(b);
    if (s == nullptr) {
      // Not able to convert to a valid deposit,
      // use wander again.
      log(
        "Reached block that is not a spawner in return behavior",
        utils::Level::Warning
      );

      setBehavior(Behavior::Wander);
      pickRandomTarget(info, x, y);

      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log(std::string("Should refill with ") + std::to_string(m_carrying));
    s->refill(m_carrying);
    m_carrying = 0.0f;

    // Re-wander again.
    setBehavior(Behavior::Wander);
    pickRandomTarget(info, x, y);

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

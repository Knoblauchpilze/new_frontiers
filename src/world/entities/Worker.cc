
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "../blocks/Deposit.hh"
# include "../blocks/SpawnerOMeter.hh"
# include "PheromonAnalyzer.hh"
# include <cxxabi.h>

namespace new_frontiers {

  Worker::Worker(const MProps& props):
    Mob(props)
  {
    setService("worker");
  }

  bool
  Worker::collect(StepInfo& info, path::Path& path) {
    // Check for enemies in the perception radius:
    // we want to flee them.
    if (checkForFlee(info, path)) {
      return true;
    }

    // The collect behavior is called when the worker
    // has spotted a deposit close enough and is now
    // moving towards it.
    // We don't need to do anything until we reach
    // the position of the deposit: then we need to
    // collect the resources and get back home.
    if (isEnRoute()) {
      // Still en route, do nothing until we reach
      // the deposit.
      return false;
    }

    // We have reached the deposit, attempt to pick
    // some resource and get back.
    BlockShPtr b = info.frustum->getClosest(
      m_tile.p,
      tiles::Portal,
      m_perceptionRadius,
      14
    );

    if (b == nullptr) {
      // For some reason the deposit does not exist,
      // return to wandering.
      pickTargetFromPheromon(info, path);
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

      pickTargetFromPheromon(info, path);
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
      std::to_string(d->getTile().p.x) + "x" + std::to_string(d->getTile().p.y)
    );

    // In case we can't fetch anything, return to
    // wander behavior.
    if (toFetch <= 0.0f) {
      log("Deposit has been emptied while en route");
      pickTargetFromPheromon(info, path);
      return true;
    }

    d->refill(-toFetch);
    carry(toFetch);

    // And then return to the colony.
    setBehavior(Behavior::Return);
    path.clear(m_tile.p);
    if (!path.generatePathTo(info, m_home, true)) {
      log("Could not generate path to colony", utils::Level::Warning);
      // Return to wandering.
      pickTargetFromPheromon(info, path);
      return true;
    }

    return true;
  }

  bool
  Worker::getBack(StepInfo& info, path::Path& path) {
    // Flee enemies.
    if (checkForFlee(info, path)) {
      return true;
    }

    // This method is called when the worker has
    // collected some resources and need to get
    // back home.
    // We need first to locate the home and set
    // the target to go there, before depositing
    // the resources in it.
    if (isEnRoute()) {
      // Still en route, do nothing until we reach
      // home.
      return false;
    }

    // We have reached home, attempt to dump the
    // resource we're transporting and get back
    // to wandering.
    world::Filter f{getOwner(), true};
    BlockShPtr b = info.frustum->getClosest(m_tile.p, tiles::Portal, -1, -1, &f);

    if (b == nullptr) {
      // For some reason the home of the entity does
      // not exist, return to wandering.
      pickTargetFromPheromon(info, path);
      return true;
    }

    SpawnerOMeterShPtr s = std::dynamic_pointer_cast<SpawnerOMeter>(b);
    if (s == nullptr) {
      // Not able to convert to a valid deposit,
      // use wander again.
      int status;
      std::string bType = abi::__cxa_demangle(typeid(*b).name(), 0, 0, &status);

      log(
        "Reached block of type \"" + bType + "\" which is not a spawner in return behavior",
        utils::Level::Warning
      );

      pickTargetFromPheromon(info, path);
      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Refilling home with " + std::to_string(m_carrying) + " resource(s)");
    s->refill(m_carrying);
    m_carrying = 0.0f;

    // Re-wander again.
    pickTargetFromPheromon(info, path);
    return true;
  }

  bool
  Worker::flee(StepInfo& /*info*/, path::Path& /*path*/) {
    // TODO: Handle this.
    return false;
  }

  bool
  Worker::wander(StepInfo& info, path::Path& path) {
    // First, we need to make sure that we won't run
    // into an ennemy.
    if (checkForFlee(info, path)) {
      return true;
    }

    // Check whether we can find any deposit in the
    // surroudings of the entity.
    BlockShPtr deposit = info.frustum->getClosest(path.cur, tiles::Portal, m_perceptionRadius, 14);

    // In case there are no deposits, continue the
    // wandering around process. We also need to
    // do that in case the first deposit is empty.
    DepositShPtr d = nullptr;
    if (deposit != nullptr) {
      d = std::dynamic_pointer_cast<Deposit>(deposit);
    }

    if (d == nullptr || d->getStock() <= 0.0f) {
      // In case we already have a target, continue
      // towards this target.
      if (isEnRoute()) {
        return false;
      }

      if (d != nullptr) {
        log("Deposit is empty");
      }

      pickTargetFromPheromon(info, path);
      return true;
    }

    // Try to generate a path to the closest deposit.
    // As the list was required to be sorted we can
    // pick the first one. We will only update the
    // path in case we can generate one: otherwise
    // we will continue with our current behavior.
    path::Path newPath = path::newPath(m_tile.p);
    std::swap(newPath, path);

    Point p = d->getTile().p;
    p.x += 0.5f;
    p.y += 0.5f;

    bool generated = newPath.generatePathTo(info, p, false);
    if (!generated) {
      log("Failed to generate path to deposit", utils::Level::Warning);
      if (isEnRoute()) {
        return false;
      }

      pickTargetFromPheromon(info, path);
      return true;
    }

    // Change to collecting behavior.
    setBehavior(Behavior::Collect);
    std::swap(newPath, path);

    return generated;
  }

  void
  Worker::pickTargetFromPheromon(StepInfo& info, path::Path& path) noexcept {
    // Generate the pheromon analyzer.
    PheromonAnalyzer pa;
    pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
    pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
    pa.setRelativeWeight(pheromon::Type::Collect, 0.4f);
    pa.setRelativeWeight(pheromon::Type::Return, 0.1f);
    pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
    pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
    pa.setRandomWeight(0.5f);

    // Generate the filtering method for pheromons.
    utils::Uuid owner = getOwner();
    auto filter = [&owner](VFXShPtr vfx) {
      return vfx->getOwner() != owner;
    };

    // Use the dedicated handler.
    if (!returnToWandering(info, filter, pa, path)) {
      log(
        "Unable to return to wandering, path could not be generated",
        utils::Level::Warning
      );
    }
  }

  bool
  Worker::checkForFlee(StepInfo& /*info*/, path::Path& /*path*/) noexcept {
    // TODO: Handle this.
    return false;
  }
}

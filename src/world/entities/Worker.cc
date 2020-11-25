
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "PheromonAnalyzer.hh"
# include "../blocks/Deposit.hh"
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
    BlockShPtr b = info.frustum->getClosest(m_tile.p, tiles::Portal, m_perceptionRadius, 14);
    if (b == nullptr) {
      // For some reason the deposit does not exist,
      // return to wandering.
      pickTargetFromPheromon(info, path, Goal::Deposit);
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

      pickTargetFromPheromon(info, path, Goal::Deposit);
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
      pickTargetFromPheromon(info, path, Goal::Deposit);
      return true;
    }

    d->refill(-toFetch);
    carry(toFetch);

    // Now we would like to get back to the colony
    // or continue searching for more resources if
    // we could only get a small amount from this
    // deposit. In both cases, this is best handled
    // by getting back to wandering: the process
    // there will be to determine which behavior
    // is the best suited.
    return wander(info, path);
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
      pickTargetFromPheromon(info, path, Goal::Home);
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

      pickTargetFromPheromon(info, path, Goal::Home);
      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Refilling home with " + std::to_string(m_carrying) + " resource(s)");
    s->refill(m_carrying);
    m_carrying = 0.0f;

    // Re-wander again.
    pickTargetFromPheromon(info, path, Goal::Deposit);
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

    // Depending on the status of the entity we will
    // prioritize a wandering behavior.
    path::Path newPath = path::newPath(m_tile.p);
    bool generated = false;

    if (availableCargo() <= 0.0f) {
      generated = wanderToHome(info, newPath);
    }
    else {
      generated = wanderToDeposit(info, newPath);
    }

    if (generated) {
      std::swap(path, newPath);
      return true;
    }

    // We couldn't generate a path: either because
    // there is no target visible to the entity or
    // because the A* failed to find a route. In
    // both cases, the best bet is to continue with
    // the current path if any.
    if (isEnRoute()) {
      return false;
    }

    // We couldn't generate a path based on the
    // most rational behavior: use a random based
    // wandering for now.
    pickTargetFromPheromon(info, path, Goal::Random);
    return true;
  }

  PheromonAnalyzer
  Worker::generateFromGoal(const Goal& goal) noexcept {
    PheromonAnalyzer pa;

    switch (goal) {
      case Goal::Deposit:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.4f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.5f);
        break;
      case Goal::Entity:
        // Workers will tend to group with workers so
        // we will focus on pheromons laid out only
        // by workers.
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.25f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.25f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.5f);
        break;
      case Goal::Home:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.4f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.5f);
        break;
      case Goal::Random:
        // Assume default is also `Random`.
      default:
        pa.setRelativeWeight(pheromon::Type::Chase, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Fight, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Collect, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Return, 0.1f);
        pa.setRelativeWeight(pheromon::Type::Wander, 0.0f);
        pa.setRelativeWeight(pheromon::Type::Flee, 0.0f);
        pa.setRandomWeight(0.8f);
        break;
    }

    return pa;
  }

  bool
  Worker::checkForFlee(StepInfo& /*info*/, path::Path& /*path*/) noexcept {
    // TODO: Handle this.
    return false;
  }
}

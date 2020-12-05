
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "PheromonAnalyzer.hh"
# include "../blocks/Deposit.hh"
# include <cxxabi.h>

namespace new_frontiers {

  Worker::Worker(const WProps& props):
    Mob(props),

    m_fleeRadiusThreshold(props.fleeRadius),
    m_fleeConeAngleSpan(props.fleeConeSpan)
  {
    setService("worker");
  }

  bool
  Worker::collect(StepInfo& info, path::Path& path) {
    // Check for enemies in the perception radius:
    // we want to flee them.
    bool ret = false;
    if (checkForFlee(info, path, ret)) {
      return ret;
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
    bool ret = false;
    if (checkForFlee(info, path, ret)) {
      return ret;
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
      // Not able to convert to a valid spawner,
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
  Worker::flee(StepInfo& info, path::Path& path) {
    // Check whether there are some enemies close enough to
    // threaten us: this will trigger the escape behavior.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> enemies = info.frustum->getVisible(
      m_tile.p,
      m_fleeRadiusThreshold,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    if (enemies.empty()) {
      return wander(info, path);
    }

    // One or more enemies have been detected and are
    // close enough to threaten us: we need to try to
    // flee in the general opposite direction of their
    // combined position.
    Point s = m_tile.p;
    Point g = newPoint();
    float w = 0.0f;

    auto weight = [&s](const Point& p) {
      return 1.0f / std::max(sk_proximityAlert, distance::d(s, p));
    };

    for (unsigned id = 0u ; id < enemies.size() ; ++id) {
      float cw = weight(enemies[id]->getTile().p);

      g.x += cw * enemies[id]->getTile().p.x;
      g.y += cw * enemies[id]->getTile().p.y;
      w += cw;
    }

    g.x /= w;
    g.y /= w;

    // Check if we're en route: if this is the case
    // but the path still likes in the escape cone
    // we will continue on that direction.
    bool inCone = distance::isInCone(
      m_tile.p,
      m_tile.p.x - g.x,
      m_tile.p.y - g.y,
      m_fleeConeAngleSpan,
      path.currentTarget()
    );
    if (isEnRoute() && (inCone || path.forced)) {
      return false;
    }

    // We now know the general direction that we should
    // avoid. We will try to flee in a cone directed in
    // the opposite direction.
    // As escaping is the main priority right now we
    // will delete any existing planned path.
    float dToEnemy = distance::d(g, m_tile.p);
    float d = info.rng.rndFloat(dToEnemy, 2.0f * dToEnemy);
    float theta = info.rng.rndAngle(0.0f, m_fleeConeAngleSpan);

    float baseAngle = distance::angleFromDirection(m_tile.p, g);

    float xDir = std::cos(baseAngle + theta - m_fleeConeAngleSpan / 2.0f);
    float yDir = std::sin(baseAngle + theta - m_fleeConeAngleSpan / 2.0f);

    // Clamp these coordinates and update the direction
    // based on that.
    info.clampPath(m_tile.p, xDir, yDir, d);
    Point t = newPoint(m_tile.p.x + d * xDir, m_tile.p.y + d * yDir);

    path::Path newPath = path::newPath(m_tile.p);
    bool generated = newPath.generatePathTo(info, t, false, m_perceptionRadius);

    // There's one caveat with the approach to
    // flee in a general direction. By assuming
    // that we want to flee from a distance of
    // at least `dToEnemy` we might reach pos
    // behind a wall or something: in this case
    // we will just fail repeatedly to generate
    // a new path.
    // In this case we will just pick a random
    // location and go there: as we reassess
    // the behavior quite often we will probably
    // find a valid position later on (at the
    // expense of some health points probably).
    int tries = 10;
    while (!generated && tries > 0) {
      d = info.rng.rndFloat(dToEnemy, 2.0f * dToEnemy);
      theta = info.rng.rndAngle(0.0f, m_fleeConeAngleSpan);

      xDir = std::cos(baseAngle + theta - m_fleeConeAngleSpan / 2.0f);
      yDir = std::sin(baseAngle + theta - m_fleeConeAngleSpan / 2.0f);

      info.clampPath(m_tile.p, xDir, yDir, d);
      t = newPoint(m_tile.p.x + d * xDir, m_tile.p.y + d * yDir);

      newPath.clear(m_tile.p);
      generated = newPath.generatePathTo(info, t, false, m_perceptionRadius);

      --tries;
    }

    // In case we didn't get a valid path, let's
    // try with a random location.
    if (!generated) {
      // Let's do also 10 tries to find a valid
      // target.
      tries = 10;
      while (!generated && tries > 0) {
        Point t;
        pickRandomTarget(info, m_tile.p, t.x, t.y);

        log("Generated random target " + std::to_string(t.x) + "x" + std::to_string(t.y));

        newPath.clear(m_tile.p, true);
        generated = newPath.generatePathTo(info, t, false, m_perceptionRadius);
        --tries;
      }
    }

    if (!generated) {
      // Failed to generate a path from random
      // motion: let's give up.
      log(
        "Failed to escape from " +
        std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y)
      );

      setBehavior(Behavior::Flee);
      return false;
    }

    log(
      "Escaping " + std::to_string(g.x) + "x" + std::to_string(g.y) +
      " from " + std::to_string(m_tile.p.x) + "x" + std::to_string(m_tile.p.y) +
      " in cone from " + std::to_string(180.0f * (baseAngle - m_fleeConeAngleSpan / 2.0f) / 3.14159f) +
      " - " + std::to_string(180.0f * (baseAngle + m_fleeConeAngleSpan / 2.0f) / 3.14159f) +
      " (d: " + std::to_string(distance::d(g, m_tile.p)) +
      ", forced: " + std::to_string(newPath.forced) + ")"
    );

    std::swap(path, newPath);
    setBehavior(Behavior::Flee);

    return true;
  }

  bool
  Worker::wander(StepInfo& info, path::Path& path) {
    // First, we need to make sure that we won't run
    // into an ennemy.
    bool ret = false;
    if (checkForFlee(info, path, ret)) {
      return ret;
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
  Worker::checkForFlee(StepInfo& info, path::Path& path, bool& pathChanged) noexcept {
    // Determine whether an ennemy entity is close
    // enough to threaten us.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> enemies = info.frustum->getVisible(
      m_tile.p,
      m_fleeRadiusThreshold,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    if (enemies.empty()) {
      return false;
    }

    pathChanged = flee(info, path);
    return true;
  }
}

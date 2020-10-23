
# include "Worker.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "../blocks/Deposit.hh"
# include "../blocks/SpawnerOMeter.hh"

# include <cxxabi.h>
# include "LocationUtils.hh"

namespace new_frontiers {

  Worker::Worker(const MProps& props):
    Mob(props)
  {
    setService("worker");
  }

  bool
  Worker::collect(StepInfo& info, float& x, float& y) {
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
      m_tile.x,
      m_tile.y,
      tiles::Portal,
      m_perceptionRadius,
      14
    );

    if (b == nullptr) {
      // For some reason the deposit does not exist,
      // return to wandering.
      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

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
      pickTargetFromPheromon(info, x, y);

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
      log("Deposit has been emptied while en route");

      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

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
    BlockShPtr b = info.frustum->getClosest(m_tile.x, m_tile.y, tiles::Portal, -1, -1, &f);

    if (b == nullptr) {
      // For some reason the home of the entity does
      // not exist, return to wandering.
      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

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

      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Refilling home with " + std::to_string(m_carrying) + " resource(s)");
    s->refill(m_carrying);
    m_carrying = 0.0f;

    // Re-wander again.
    setBehavior(Behavior::Wander);
    pickTargetFromPheromon(info, x, y);

    return true;
  }

  bool
  Worker::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    BlockShPtr deposit = info.frustum->getClosest(x, y, tiles::Portal, m_perceptionRadius, 14);

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

      pickTargetFromPheromon(info, x, y);

      return true;
    }

    // Change to collecting behavior.
    setBehavior(Behavior::Collect);

    // Assign the target to the closest deposit:
    // as we requested the list to be sorted we
    // can pick the first one.
    x = d->getTile().x + 0.5f;
    y = d->getTile().y + 0.5f;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

  void
  Worker::pickTargetFromPheromon(StepInfo& info, float& x, float& y) noexcept {
    // Collect the pheromons laid out by colleagues
    // of this worker.
    world::Filter f{getOwner(), true};
    tiles::Effect* te = nullptr;
    std::vector<VFXShPtr> d = info.frustum->getVisible(x, y, m_perceptionRadius, te, -1, &f);

    // We will need a random target so better compute
    // it right now.
    float xRnd = x, yRnd = y;
    pickRandomTarget(info, xRnd, yRnd);

    // In case no pheromons are visible use the
    // default wandering behavior.
    if (d.empty()) {
      x = xRnd;
      y = yRnd;

      return;
    }

    // Otherwise, see what kind of pheromones are
    // visible.
    PheromonInfo coPI{0.0f, 0.0f, 0.0f, 0}, rePI{0.0f, 0.0f, 0.0f, 0};

    for (unsigned id = 0u ; id < d.size() ; ++id) {
      VFXShPtr v = d[id];

      PheromonShPtr p = std::dynamic_pointer_cast<Pheromon>(v);
      if (v == nullptr || p == nullptr) {
        continue;
      }

      switch (p->getType()) {
        case pheromon::Type::Collect:
          coPI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Return:
          rePI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Chase:
        case pheromon::Type::Fight:
        case pheromon::Type::Wander:
        default:
          // Don't accumulate irrelevant pheromon.
          break;
      }
    }

    coPI.normalize();
    rePI.normalize();

    // In case no pheromons of the right type were
    // found, use the default wandering behavior.
    // The goal will be to compute a weighted sum
    // of the barycenter of visible pheromons and
    // a random location.
    // This will allow the pheromons to have some
    // sort of influence but to not be the only
    // way to select the next target. The precise
    // weights used to modulate the picked target
    // are updated based on whether some pheromons
    // are actually visible.
    float wRnd = 0.5f;
    coPI.w = 0.4f;
    rePI.w = 1.0f - wRnd - coPI.w;

    if (coPI.count == 0u && rePI.count == 0u) {
      wRnd = 1.0f; coPI.w = 0.0f; rePI.w = 0.0f;
    }
    if (coPI.count == 0u) {
      float s = 1.0f - coPI.w;
      wRnd /= s; rePI.w /= s;
      coPI.w = 0.0f;
    }
    if (rePI.count == 0u) {
      float s = 1.0f - rePI.w;
      wRnd /= s; coPI.w /= s;
      rePI.w = 0.0f;
    }

    x = (wRnd * xRnd + coPI.w * coPI.x + rePI.w * rePI.x) / (wRnd + coPI.w + rePI.w);
    y = (wRnd * yRnd + coPI.w * coPI.y + rePI.w * rePI.y) / (wRnd + coPI.w + rePI.w);

    log(
      "Barycenter of collect is at " + std::to_string(coPI.x) + "x" +  std::to_string(coPI.y) +
      ", return is " + std::to_string(rePI.x) + "x" +  std::to_string(rePI.y) +
      ", random is " + std::to_string(xRnd) + "x" +  std::to_string(yRnd) +
      " final coord is " + std::to_string(x) + "x" + std::to_string(y),
      utils::Level::Verbose
    );

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);
  }

}

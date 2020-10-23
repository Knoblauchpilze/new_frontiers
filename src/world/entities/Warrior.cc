
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace new_frontiers {

  Warrior::Warrior(const WProps& props):
    Mob(props),

    m_attack(props.attack),
    m_attackCost(props.attackCost)
  {
    setService("warrior");
  }

  bool
  Warrior::chase(StepInfo& info, float& x, float& y) {
    // In chase mode we need to pick the closest entity
    // and set its current position as the new target
    // for this entity.
    // In case no entities can be found, we will return
    // to the wandering behavior.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(
      x,
      y,
      m_perceptionRadius,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    if (entities.empty()) {
      // Couldn't find the entity we were chasing, get
      // back to wander behavior.
      log("Lost entity at " + std::to_string(x) + "x" + std::to_string(y));

      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

      return true;
    }

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Update the target with the actual position of
    // the entity: indeed the entity may be moving
    // so we want to accurately chase it.
    x = e->getTile().x;
    y = e->getTile().y;

    // In case we are close enough of the entity to
    // actually hit it, do so if we are able to.
    if (!isEnRoute() && m_energy >= m_attackCost) {
      bool alive = e->damage(m_attack);
      log("Attacking for " + std::to_string(m_attack) + " damage, " + std::to_string(e->getHealthRatio()) + " health ratio");

      m_energy -= m_attackCost;

      // Mark the entity for deletion in case it is
      // now dead. We will also return back to the
      // wandering behavior.
      if (!alive) {
        log("Killed entity at " + std::to_string(x) + "x" + std::to_string(y));

        info.removeEntity(e.get());

        setBehavior(Behavior::Wander);
        pickTargetFromPheromon(info, x, y);

        return true;
      }
    }

    return true;
  }

  bool
  Warrior::getBack(StepInfo& info, float& x, float& y) {
    // The get back behavior is active whenever the
    // warrior needs to get back home.
    if (isEnRoute()) {
      // Still en route, do nothing until we reach
      // home.
      return false;
    }

    // We have reached home, do nothing and wait for
    // new instructions. We need to make sure that
    // we are actually close to home (and that it did
    // not get destroyed for some reasons).
    world::Filter f{getOwner(), true};
    world::ItemEntry ie = info.frustum->getClosest(m_tile.x, m_tile.y, world::ItemType::Block, f);
    world::Block b;

    if (ie.index >= 0 && ie.type == world::ItemType::Block) {
      b = info.frustum->block(ie.index);
    }

    if (ie.index < 0 || ie.type != world::ItemType::Block || b.tile.type != tiles::Portal) {
      setBehavior(Behavior::Wander);
      pickTargetFromPheromon(info, x, y);

      return true;
    }

    // Refill the home spawner with the amount we
    // scraped from the deposit.
    log("Reached home, now retired");

    return false;
  }

  bool
  Warrior::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> entities = info.frustum->getVisible(
      x,
      y,
      m_perceptionRadius,
      te,
      -1,
      &f,
      world::Sort::Distance
    );

    // In case there are no entities, continue the
    // wandering around process.
    if (entities.empty()) {
      // In case we already have a target, continue
      // towards this target.
      if (isEnRoute()) {
        return false;
      }

      pickTargetFromPheromon(info, x, y);

      return true;
    }

    // Change to chasing behavior.
    setBehavior(Behavior::Chase);

    // Pick the first one as it will be the closest.
    EntityShPtr e = entities.front();

    // Assign the target to the closest entities:
    // as we requested the list to be sorted we
    // can pick the first one.
    x = e->getTile().x;
    y = e->getTile().y;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

  void
  Warrior::pickTargetFromPheromon(StepInfo& info, float& x, float& y) noexcept {
    // Collect the pheromons laid out by ennemis of
    // this warrior.
    world::Filter f{getOwner(), false};
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
    // visible: we will give some priority to the
    // fighting pheromon over other types and so
    // on to specialize the behavior.
    PheromonInfo chPI{0.0f, 0.0f, 0.0f, 0};
    PheromonInfo fiPI{0.0f, 0.0f, 0.0f, 0};
    PheromonInfo coPI{0.0f, 0.0f, 0.0f, 0};
    PheromonInfo rePI{0.0f, 0.0f, 0.0f, 0};
    PheromonInfo waPI{0.0f, 0.0f, 0.0f, 0};

    for (unsigned id = 0u ; id < d.size() ; ++id) {
      VFXShPtr v = d[id];

      PheromonShPtr p = std::dynamic_pointer_cast<Pheromon>(v);
      if (v == nullptr || p == nullptr) {
        continue;
      }

      switch (p->getType()) {
        case pheromon::Type::Chase:
          chPI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Fight:
          fiPI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Collect:
          coPI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Return:
          rePI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        case pheromon::Type::Wander:
          waPI.accumulate(p->getTile().x, p->getTile().y, p->getAmount());
          break;
        default:
          // Don't accumulate unknown pheromon.
          break;
      }
    }

    chPI.normalize();
    fiPI.normalize();
    coPI.normalize();
    rePI.normalize();
    waPI.normalize();

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
    float wRnd = 0.2f;
    fiPI.w = 0.2f;
    waPI.w = 0.2f;
    coPI.w = 0.15f;
    rePI.w = 0.15f;
    chPI.w = 1.0f - wRnd - fiPI.w - waPI.w - coPI.w - rePI.w;

    if (fiPI.count == 0 && chPI.count == 0 && coPI.count == 0 && rePI.count == 0 && waPI.count == 0) {
      wRnd = 1.0f; chPI.w = 0.0f; fiPI.w = 0.0f; coPI.w = 0.0f; rePI.w = 0.0f; waPI.w = 0.0f;
    }
    if (fiPI.count == 0) {
      float s = 1.0f - fiPI.w;
      wRnd /= s; chPI.w /= s; coPI.w /= s; rePI.w /= s; waPI.w /= s;
      fiPI.w = 0.0f;
    }
    if (chPI.count == 0) {
      float s = 1.0f - chPI.w;
      wRnd /= s; fiPI.w /= s; coPI.w /= s; rePI.w /= s; waPI.w /= s;
      chPI.w = 0.0f;
    }
    if (coPI.count == 0) {
      float s = 1.0f - coPI.w;
      wRnd /= s; fiPI.w /= s; chPI.w /= s; rePI.w /= s; waPI.w /= s;
      coPI.w = 0.0f;
    }
    if (rePI.count == 0) {
      float s = 1.0f - rePI.w;
      wRnd /= s; fiPI.w /= s; chPI.w /= s; coPI.w /= s; waPI.w /= s;
      rePI.w = 0.0f;
    }
    if (waPI.count == 0) {
      float s = 1.0f - waPI.w;
      wRnd /= s; fiPI.w /= s; chPI.w /= s; coPI.w /= s; rePI.w /= s;
      waPI.w = 0.0f;
    }

    float totW = fiPI.w + waPI.w + coPI.w + rePI.w + chPI.w;
    x = (xRnd + wRnd + chPI.x * chPI.w + fiPI.x * fiPI.w + coPI.x * coPI.w + rePI.x * rePI.w + waPI.x * waPI.w) / totW;
    y = (yRnd + wRnd + chPI.y * chPI.w + fiPI.y * fiPI.w + coPI.y * coPI.w + rePI.y * rePI.w + waPI.y * waPI.w) / totW;

    log(
      "rn: " + std::to_string(xRnd) + "x" +  std::to_string(yRnd) +
      "ch: " + std::to_string(chPI.x) + "x" +  std::to_string(chPI.y) +
      ", fi: " + std::to_string(fiPI.x) + "x" +  std::to_string(fiPI.y) +
      ", co: " + std::to_string(coPI.x) + "x" +  std::to_string(coPI.y) +
      ", re: " + std::to_string(rePI.x) + "x" +  std::to_string(rePI.y) +
      ", wa: " + std::to_string(waPI.x) + "x" +  std::to_string(waPI.y) +
      ", fi: " + std::to_string(x) + "x" + std::to_string(y) +
      " (weights: " + std::to_string(wRnd) + ", "+ std::to_string(chPI.w) +
      ", " + std::to_string(fiPI.w) + ", " + std::to_string(coPI.x) + ", " +
      std::to_string(rePI.w) + ", " + std::to_string(waPI.w) + ")",
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

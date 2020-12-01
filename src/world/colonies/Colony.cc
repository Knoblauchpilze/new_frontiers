
# include "Colony.hh"
# include "StepInfo.hh"
# include "../blocks/SpawnerOMeter.hh"
# include "../blocks/BlockFactory.hh"

namespace new_frontiers {

  Colony::Colony(const Props& props):
    WorldElement(props.id.toString(), props.id),

    m_home(props.home),

    m_focus(props.focus),

    m_budget(props.budget),
    m_actionCost(props.actionCost),
    m_refill(props.refill),

    m_radius(props.radius),
    m_maxSize(props.maxSize),
    m_size(0),

    m_active(false),

    m_peaceToWarThreshold(props.warThreshold)
  {
    setService("colony");

    if (!props.id.valid()) {
      error(
        "Unable to create colony from uuid",
        "Invalid uuid"
      );
    }
  }

  void
  Colony::update(const StepInfo& info) {
    // Update the budget available for the colony's
    // thinking process based on the elapsed time.
    m_budget += info.elapsed * m_refill;
  }

  void
  Colony::changeFocus(const StepInfo& info) {
    // Cound how many entities are visible in the
    // reserved space of the colony.
    world::Filter f{getOwner(), false};
    tiles::Entity* te = nullptr;
    std::vector<EntityShPtr> enemies = info.frustum->getVisible(m_home, m_radius, te, -1, &f);

    // In case the threshold is reached, switch to
    // war mode.
    colony::Priority p = m_focus;

    if (enemies.size() > m_peaceToWarThreshold) {
      m_focus = colony::Priority::War;
    }
    else {
      m_focus = colony::Priority::Expansion;
    }

    if (p != m_focus) {
      switch (m_focus) {
        case colony::Priority::War:
          log("Colony is now at war");
          break;
        case colony::Priority::Expansion:
          log("Colony is now at expansion");
          break;
        case colony::Priority::Consolidation:
          log("Colony is now at consolidation");
          break;
        default:
          log("Colony is now in unknown state");
          break;
      }
    }
  }

  void
  Colony::think(StepInfo& info) {
    // Make sure the budget is enough to spawn a
    // new portal based on the focus.
    if (m_budget < m_actionCost || m_size >= m_maxSize) {
      return;
    }

    // Generate a position in the radius of the
    // colony. Note that this position will be
    // set to the `home` position of the colony
    // in case it's the first spawner.
    float x = m_home.x;
    float y = m_home.y;

    while (info.frustum->obstructed(x, y)) {
      float r = info.rng.rndFloat(0, m_radius * m_radius);
      float theta = info.rng.rndAngle();

      x = m_home.x + std::round(std::sqrt(r) * std::cos(theta));
      y = m_home.y + std::round(std::sqrt(r) * std::sin(theta));

      info.clampCoord(x, y);
    }

    log("Spawning portal at " + std::to_string(x) + "x" + std::to_string(y));

    // Spawn a block corresponding to the current
    // focus of the colony.
    SpawnerOMeter::SOMProps pp = BlockFactory::newSpawnerOMeterProps(x, y);
    pp.owner = getOwner();

    switch (m_focus) {
      case colony::Priority::Expansion:
        pp.mob = tiles::IncaOverlord;
        break;
      case colony::Priority::War:
        pp.mob = tiles::DemonBat;
        pp.agent = mob::Type::Warrior;
        break;
      case colony::Priority::Consolidation:
        // Default mode is consolidation.
      default:
        pp.mob = tiles::Executioner;
        break;
    }

    pp.refill = 0.0f;
    pp.threshold *= 2.0f;
    pp.reserve = pp.threshold;

    // Spend the budget and create the spawner.
    m_budget -= m_actionCost;
    ++m_size;

    BlockShPtr b = BlockFactory::newSpawnerOMeter(pp);
    info.spawnBlock(b);
  }

}

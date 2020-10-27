
# include "Colony.hh"
# include "StepInfo.hh"
# include "../blocks/SpawnerOMeter.hh"
# include "../blocks/BlockFactory.hh"

namespace new_frontiers {

  Colony::Colony(const Props& props):
    WorldElement(props.id.toString(), props.id),

    m_homeX(props.homeX),
    m_homeY(props.homeY),

    m_focus(props.focus),

    m_budget(props.budget),
    m_actionCost(props.actionCost),
    m_refill(props.refill),

    m_radius(props.radius),
    m_maxSize(props.maxSize),
    m_size(0),

    m_active(false)
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
  Colony::changeFocus(const StepInfo& /*info*/) {
    // TODO: Implement this.
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
    float x = m_homeX;
    float y = m_homeY;

    while (info.frustum->obstructed(x, y)) {
      float r = info.rng.rndFloat(0, m_radius * m_radius);
      float theta = info.rng.rndAngle();

      x = m_homeX + std::round(std::sqrt(r) * std::cos(theta));
      y = m_homeY + std::round(std::sqrt(r) * std::sin(theta));

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

    // Spend the budget and create the spawner.
    m_budget -= m_actionCost;
    ++m_size;

    BlockShPtr b = BlockFactory::newSpawnerOMeter(pp);
    info.spawnBlock(b);
  }

}

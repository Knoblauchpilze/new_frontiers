
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
    m_refill(props.refill)
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
  Colony::think(StepInfo& info) {
    // Make sure the budget is enough to spawn a
    // new portal based on the focus.
    if (m_budget < m_actionCost) {
      return;
    }

    // Spawn a block corresponding to the current
    // focus of the colony.
    SpawnerOMeter::SOMProps pp;

    switch (m_focus) {
      case colony::Priority::Expansion:
        pp = BlockFactory::newSpawnerOMeterProps(
          m_homeX,
          m_homeY,
          tiles::IncaOverlord
        );
        break;
      case colony::Priority::War:
        pp = BlockFactory::newSpawnerOMeterProps(
          m_homeX,
          m_homeY,
          tiles::DemonBat
        );

        pp.agent = mob::Type::Warrior;
        break;
      case colony::Priority::Consolidation:
        // Default mode is consolidation.
      default:
        pp = BlockFactory::newSpawnerOMeterProps(
          m_homeX,
          m_homeY,
          tiles::Executioner
        );
        break;
    }

    // Spend the budget and create the spawner.
    m_budget -= m_actionCost;

    BlockShPtr b = BlockFactory::newSpawnerOMeter(pp);
    info.spawnBlock(b);
  }

}


# include "Colony.hh"
# include "StepInfo.hh"
# include "../blocks/SpawnerOMeter.hh"
# include "../blocks/BlockFactory.hh"

namespace new_frontiers {

  Colony::Colony(const utils::Uuid& uuid,
                 float x,
                 float y):
    WorldElement(uuid.toString(), uuid),

    m_homeX(x),
    m_homeY(y),

    m_focus(colony::Priority::Expansion),

    m_budget(10.0f)
  {
    setService("colony");

    if (!uuid.valid()) {
      error(
        "Unable to create colony from uuid",
        "Invalid uuid"
      );
    }
  }

  void
  Colony::step(StepInfo& info) {
    // TODO: Implement this.
    // If the colony spent its budget, we can't
    // do anything more.
    if (m_budget <= 0.0f) {
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
      default:
        pp = BlockFactory::newSpawnerOMeterProps(
          m_homeX,
          m_homeY,
          tiles::Executioner
        );
        break;
    }

    // Spend the budget and create the spawner.
    m_budget = 0.0f;
    BlockShPtr b = BlockFactory::newSpawnerOMeter(pp);

    // And spawn the block.
    info.spawnBlock(b);
  }

}

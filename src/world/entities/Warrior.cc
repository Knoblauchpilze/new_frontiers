
# include "Warrior.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace new_frontiers {

  Warrior::Warrior(const EntityTile& tile,
                   float homeX,
                   float homeY):
    Mob(tile, homeX, homeY),

    m_attack(2.0f),

    m_attackDelay(toMilliseconds(500)),
    m_last(now() - m_attackDelay),
    m_passed()
  {
    setService("warrior");
  }

  bool
  Warrior::chase(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement this.
    return false;
  }

  bool
  Warrior::fight(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement this.
    return false;
  }

  bool
  Warrior::getBack(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // TODO: Implement this.
    return false;
  }

  bool
  Warrior::wander(StepInfo& info, float& x, float& y) {
    // Check whether we can find any deposit in the
    // surroudings of the entity.
    std::vector<EntityShPtr> entities = info.frustum->getEntities(
      x,
      y,
      m_perceptionRadius,
      tiles::Executioner,
      0,
      true
    );

    // In case there are no entities, continue the
    // wandering around process.
    // TODO: Should take pheromon into account.
    if (entities.empty()) {
      pickRandomTarget(info, x, y);

      return true;
    }

    // Change to chasing behavior.
    setBehavior(Behavior::Chase);

    // Assign the target to the closest entities:
    // as we requested the list to be sorted we
    // can pick the first one.
    x = entities.front()->getTile().x;
    y = entities.front()->getTile().y;

    // Update debug elements.
    m_cPoints.clear();
    m_cPoints.push_back(m_tile.x);
    m_cPoints.push_back(m_tile.y);
    m_cPoints.push_back(x);
    m_cPoints.push_back(y);

    return true;
  }

}

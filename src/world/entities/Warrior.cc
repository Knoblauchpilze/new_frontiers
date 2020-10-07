
# include "Warrior.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  Warrior::Warrior(const EntityTile& tile,
                   float homeX,
                   float homeY):
    Mob(tile, homeX, homeY)
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
  Warrior::wander(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Use the base handler.
    // pickRandomTarget(info, x, y);

    // TODO: Reactivate this.
    return false;
  }

}

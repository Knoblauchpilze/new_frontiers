
# include "Warrior.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  Warrior::Warrior(const EntityTile& tile):
    Mob(tile)
  {
    setService("warrior");
  }

  bool
  Warrior::wander(StepInfo& /*info*/, float& /*x*/, float& /*y*/) {
    // Use the base handler.
    // pickRandomTarget(info, x, y);

    // TODO: Reactivate this.
    return false;
  }

}

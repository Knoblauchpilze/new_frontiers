
# include "Mob.hh"

namespace new_frontiers {

  Mob::Mob(const EntityTile& tile):
    Entity(tile, 0.5f),

    m_vfxDelay(toMilliseconds(8000)),
    m_last(now() - m_vfxDelay),

    m_passed(),

    m_behavior(Behavior::Wander)
  {}

  bool
  Mob::takeAction(StepInfo& info, float& x, float& y) {
    // First, we need to update the behavior of the
    // entity: it is relevant because we just reached
    // the destination we previously picked.
    Thought t = behave(info, x, y);

    // Once this is done, we may need to emit a new
    // pheromon: this allow to make sure that we
    // indicate any change of state for this entity.
    if (t.behaviorChanged) {
      emitPheromon(info);
    }

    return t.actionTaken;
  }

}

#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace new_frontiers {

  inline
  Spawner::Spawner(const BlockTile& tile,
                   float radius,
                   const tiles::Entity& mob,
                   int id):
    Block(tile, "spawner"),

    m_mob(mob),
    m_mobID(id),

    m_radius(std::max(radius, 0.0f))
  {}

  inline
  bool
  Spawner::step(StepInfo& info) {
    // Check whether the spawner is allowed to spawn
    // a new entity at this frame.
    update(info);

    // In case a new entity cannot be spawned, we're
    // done.
    if (!canSpawn(info)) {
      return false;
    }

    // Spawn a new entity and prepare it.
    EntityShPtr ent = spawn(info);
    preSpawn(info, ent);

    info.eSpawned.push_back(ent);

    return true;
  }

}

#endif    /* SPAWNER_HXX */

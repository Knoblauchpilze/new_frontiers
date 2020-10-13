#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace new_frontiers {

  namespace mob {

    inline
    Type
    strToType(const std::string& str, bool& error) noexcept {
      // Assume no error.
      error = false;

      if (str == "Worker") {
        return Type::Worker;
      }
      if (str == "Warrior") {
        return Type::Warrior;
      }

      // Return the default `Worker` value.
      error = true;
      return Type::Worker;
    }

  }

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
    if (ent == nullptr) {
      log("Spawner generated null entity, discarding it");
      return false;
    }

    preSpawn(info, ent);

    info.spawnEntity(ent);

    return true;
  }

}

#endif    /* SPAWNER_HXX */

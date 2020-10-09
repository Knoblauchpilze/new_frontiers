
# include "Spawner.hh"
# include "../entities/Mob.hh"
# include "../entities/Worker.hh"
# include "../entities/Warrior.hh"

namespace new_frontiers {

  EntityShPtr
  Spawner::spawn(StepInfo& info) noexcept {
    EntityTile e;

    e.type = m_mob;
    e.id = m_mobID;

    // Spawn the entity within `radius` of the spawner,
    // using the provided rng to pick a point. Don't
    // forget to add the position of the spawner itself.
    float r = info.rng.rndFloat(0, m_radius * m_radius);
    float theta = info.rng.rndAngle();

    e.x = std::sqrt(r) * std::cos(theta);
    e.y = std::sqrt(r) * std::sin(theta);

    e.x += m_tile.x;
    e.y += m_tile.y;

    // Clamp the coordinates to be inside the world's
    // boundaries.
    info.clampCoord(e.x, e.y);

    // Depending on the agent's to plug to the bodies,
    // create the corresponding pointer.
    MobShPtr mp = nullptr;

    switch (m_type) {
      case mob::Type::Warrior:
        mp = std::make_shared<Warrior>(e, m_tile.x + 0.5f, m_tile.y + 0.5f);
        break;
      case mob::Type::Worker:
        // Assume default type is a worker.
      default:
        mp = std::make_shared<Worker>(e, m_tile.x + 0.5f, m_tile.y + 0.5f);
        break;
    }

    return mp;
  }

}

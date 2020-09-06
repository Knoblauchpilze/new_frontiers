
# include "Spawner.hh"
# include "../entities/HostileMob.hh"

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

    return std::make_shared<HostileMob>(e);
  }

}

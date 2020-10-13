
# include "Spawner.hh"
# include "../entities/Mob.hh"
# include "../entities/Worker.hh"
# include "../entities/Warrior.hh"

namespace new_frontiers {

  Spawner::Spawner(const SProps& props):
    Block(props, "spawner"),

    m_mob(props.mob),
    m_type(props.agent),
    m_mobID(props.mVariant),

    m_spawnRadius(std::max(props.spawnRadius, 0.0f))
  {}

  EntityShPtr
  Spawner::spawn(StepInfo& info) noexcept {
    EntityTile e;

    e.type = m_mob;
    e.id = m_mobID;

    // Spawn the entity within `radius` of the spawner,
    // using the provided rng to pick a point. Don't
    // forget to add the position of the spawner itself.
    float r = info.rng.rndFloat(0, m_spawnRadius * m_spawnRadius);
    float theta = info.rng.rndAngle();

    e.x = std::sqrt(r) * std::cos(theta);
    e.y = std::sqrt(r) * std::sin(theta);

    e.x += (m_tile.x + 0.5f);
    e.y += (m_tile.y + 0.5f);

    // Clamp the coordinates to be inside the world's
    // boundaries.
    info.clampCoord(e.x, e.y);

    // Depending on the agent's to plug to the bodies,
    // create the corresponding pointer.
    MobShPtr mp = nullptr;

    switch (m_type) {
      case mob::Type::Warrior: {
        Warrior::WProps pp;
        pp.tile = e;
        pp.radius = 0.5f;

        pp.perception = 4.0f;
        pp.health = 12.0f;

        pp.arrival = 0.01f;
        pp.pathLength = 3.0f;

        pp.homeX = m_tile.x + 0.5f;
        pp.homeY = m_tile.y + 0.5f;

        pp.carrying = 0.0f;
        pp.cargo = 10.0f;

        pp.vfxDelay = toMilliseconds(3000);

        pp.attack = 2.0f;
        pp.attackDelay = toMilliseconds(2000);

        mp = std::make_shared<Warrior>(pp);
        } break;
      case mob::Type::Worker:
        // Assume default type is a worker.
      default: {
        Mob::MProps pp;
        pp.tile = e;
        pp.radius = 0.5f;

        pp.perception = 4.0f;
        pp.health = 12.0f;

        pp.arrival = 0.01f;
        pp.pathLength = 3.0f;

        pp.homeX = m_tile.x + 0.5f;
        pp.homeY = m_tile.y + 0.5f;

        pp.carrying = 0.0f;
        pp.cargo = 10.0f;

        pp.vfxDelay = toMilliseconds(3000);

        mp = std::make_shared<Worker>(pp);
        } break;
    }

    return mp;
  }

}

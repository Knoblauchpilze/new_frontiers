
# include "Spawner.hh"
# include "../entities/Mob.hh"
# include "../entities/Worker.hh"
# include "../entities/Warrior.hh"
# include "../entities/EntityFactory.hh"

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
    // Spawn the entity within `radius` of the spawner,
    // using the provided rng to pick a point. Don't
    // forget to add the position of the spawner itself.
    float r = info.rng.rndFloat(0, m_spawnRadius * m_spawnRadius);
    float theta = info.rng.rndAngle();

    float x = std::sqrt(r) * std::cos(theta);
    float y = std::sqrt(r) * std::sin(theta);

    x += (m_tile.p.x + 0.5f);
    y += (m_tile.p.y + 0.5f);

    // Clamp the coordinates to be inside the world's
    // boundaries.
    info.clampCoord(x, y);

    // Depending on the agent's to plug to the bodies,
    // create the corresponding pointer.
    MobShPtr mp = nullptr;

    switch (m_type) {
      case mob::Type::Warrior: {
        Warrior::WProps pp = EntityFactory::newWarriorProps(x, y, m_mob);
        pp.tile.id = m_mobID;

        pp.homeX = m_tile.p.x + 0.5f;
        pp.homeY = m_tile.p.y + 0.5f;

        pp.owner = getOwner();

        mp = std::make_shared<Warrior>(pp);
        } break;
      case mob::Type::Worker:
        // Assume default type is a worker.
      default: {
        Mob::MProps pp = EntityFactory::newWorkerProps(x, y, m_mob);
        pp.tile.id = m_mobID;

        pp.homeX = m_tile.p.x + 0.5f;
        pp.homeY = m_tile.p.y + 0.5f;

        pp.owner = getOwner();

        pp.perception = 7.0f;

        mp = std::make_shared<Worker>(pp);
        } break;
    }

    return mp;
  }

}

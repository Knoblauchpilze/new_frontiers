#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace new_frontiers {

  inline
  Spawner::Spawner(const SolidTile& tile):
    SolidElement(tile, "spawner"),

    m_interval(toMilliseconds(1000)),
    m_last(now() - m_interval),

    m_toSpawn(10),
    m_spawned(0),

    m_mob(Hydra),
    m_mobID(0),

    m_radius(6.0f)
  {}

  inline
  bool
  Spawner::step(StepInfo& info) {
    // THe main goal of the spawner is to create new
    // entities to send in the world. We rely on the
    // fact that the spawner might be depleted or not
    // able to spawn a new entity yet to proceed.
    if (depleted() || !canSpawn(info.moment)) {
      return false;
    }

    // Spawn a new entity.
    info.eSpawned.push_back(spawn(info.rng));

    return true;
  }

  inline
  bool
  Spawner::depleted() const noexcept {
    return m_toSpawn <= 0;
  }

  inline
  bool
  Spawner::canSpawn(const TimeStamp& moment) const noexcept {
    return moment - m_interval >= m_last;
  }

  inline
  EntityShPtr
  Spawner::spawn(RNG& rng) noexcept {
    EntityTile e;

    e.type = m_mob;
    e.id = m_mobID;

    // Spawn the entity within `radius` of the spawner,
    // using the provided rng to pick a point. Don't
    // forget to add the position of the spawner itself.
    float r = rng.rndFloat(0, m_radius * m_radius);
    float theta = rng.rndAngle();

    e.x = std::sqrt(r) * std::cos(theta);
    e.y = std::sqrt(r) * std::sin(theta);

    e.x += m_tile.x;
    e.y += m_tile.y;

    // Randomize the effect to give to the entities.
    Effect vfx = (Effect)rng.rndInt(0, EffectsCount - 1);

    // This is the last time the spawner
    // has been activated.
    ++m_spawned;
    --m_toSpawn;
    m_last = now();

    return std::make_shared<Entity>(e, vfx);
  }

}

#endif    /* SPAWNER_HXX */

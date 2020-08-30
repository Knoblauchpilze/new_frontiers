#ifndef    WORLD_ELEMENTS_HXX
# define   WORLD_ELEMENTS_HXX

# include "WorldElements.hh"

namespace new_frontiers {

  template <typename TileType>
  inline
  const Tile<TileType>&
  WorldElement<TileType>::getDesc() const noexcept {
    return m_tile;
  }

  template <typename TileType>
  inline
  WorldElement<TileType>::WorldElement(const Tile<TileType>& desc,
                                       const std::string& name):
    utils::CoreObject(name),

    m_tile(desc)
  {
    setService("element");
  }

  inline
  Entity::Entity(const EntityTile& desc):
    WorldElement(desc, "entity")
  {}

  inline
  SolidElement::SolidElement(const SolidTile& desc,
                             const std::string& name):
    WorldElement<Sprite>(desc, name)
  {}

  inline
  Spawner::Spawner(const SolidTile& desc):
    SolidElement(desc, "spawner"),

    m_interval(toMilliseconds(1000)),
    m_last(now() - m_interval),

    m_toSpawn(10),
    m_spawned(0),

    m_mob(Hydra),
    m_mobID(0),

    m_radius(6.0f)
  {}

  inline
  void
  Spawner::step(std::vector<EntityShPtr>& created, RNG& rng) {
    // THe main goal of the spawner is to create new
    // entities to send in the world. We rely on the
    // fact that the spawner might be depleted or not
    // able to spawn a new entity yet to proceed.
    if (depleted() || !canSpawn()) {
      return;
    }

    // Spawn a new entity.
    created.push_back(spawn(rng));
  }

  inline
  bool
  Spawner::depleted() const noexcept {
    return m_toSpawn <= 0;
  }

  inline
  bool
  Spawner::canSpawn() const noexcept {
    return now() - m_interval >= m_last;
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

    e.x = static_cast<int>(std::sqrt(r) * std::cos(theta));
    e.y = static_cast<int>(std::sqrt(r) * std::sin(theta));

    e.x += m_tile.x;
    e.y += m_tile.y;

    // This is the last time the spawner
    // has been activated.
    ++m_spawned;
    --m_toSpawn;
    m_last = now();

    return std::make_shared<Entity>(e);
  }

}

#endif    /* WORLD_ELEMENTS_HXX */

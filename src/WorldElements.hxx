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
  WorldElement<TileType>::WorldElement(const Tile<TileType>& desc):
    m_tile(desc)
  {}

  inline
  SolidElement::SolidElement(const SolidTile& desc):
    WorldElement<Sprite>(desc)
  {}

  inline
  Entity::Entity(const EntityTile& desc):
    WorldElement(desc)
  {}

  inline
  Spawner::Spawner(const SolidTile& desc):
    SolidElement(desc),

    m_interval(toMilliseconds(1000)),
    m_last(now() - m_interval),

    m_toSpawn(10),
    m_spawned(0),

    m_mob(Hydra),
    m_mobID(0),

    m_radius(0.0f)
  {}

  inline
  bool
  Spawner::depleted() const noexcept {
    return m_toSpawn <= 0;
  }

  inline
  bool
  Spawner::canSpawn(const TimeStamp& now) const noexcept {
    return now - m_interval >= m_last;
  }

  inline
  Entity
  Spawner::spawn(RNG& rng) noexcept {
    EntityTile e;

    e.type = m_mob;
    e.id = m_mobID;

    // Spawn the entity within `radius` of
    // the spawner, using the provided rng
    // to pick a point.
    float r = rng.rndFloat(0, m_radius);
    float theta = rng.rndAngle();

    e.x = std::sqrt(r) * std::cos(theta);
    e.y = std::sqrt(r) * std::cos(theta);

    return Entity(e);
  }

}

#endif    /* WORLD_ELEMENTS_HXX */

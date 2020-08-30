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
  VFX::VFX(const VFXTile& desc):
    WorldElement(desc, "vfx"),

    m_transitions(2),

    m_decay(toMilliseconds(250)),
    m_lastDecay(toMilliseconds(500)),

    m_phase(now() + m_decay)
  {}

  inline
  bool
  VFX::step(RNG& /*rng*/) {
    // Check whether the vfx should decay in its
    // next form.
    if (now() < m_phase) {
      return false;
    }

    // If we reached the last transition, it's
    // time for this effect to disappear.
    if (m_transitions <= 0) {
      return true;
    }

    // Otherwise, move to the next phase for
    // this vfx and reset the decay time.
    ++m_tile.id;
    --m_transitions;

    if (m_transitions > 0) {
      m_phase = now() + m_decay;
    }
    else {
      m_phase = now() + m_lastDecay;
    }

    return false;
  }

  inline
  Entity::Entity(const EntityTile& desc,
                 const Effect& vfx):
    WorldElement(desc, "entity"),

    m_vfx(vfx),

    m_vfxDelta(toMilliseconds(2000)),
    m_last(now() - m_vfxDelta)
  {}

  inline
  void
  Entity::step(std::vector<VFXShPtr>& created, RNG& /*rng*/) {
    // Check whether we shoud spawn a new vfx.
    if (now() - m_vfxDelta < m_last || true) {
      return;
    }

    // Now is the last time we generated a vfx
    // for this entity.
    m_last = now() + m_vfxDelta;

    // Generate a new vfx at the location of
    // the entity.
    VFXTile v;

    v.type = m_vfx;
    v.id = 0;
    v.x = m_tile.x;
    v.y = m_tile.y;

    created.push_back(std::make_shared<VFX>(v));
  }

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

    m_toSpawn(1),
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

#endif    /* WORLD_ELEMENTS_HXX */

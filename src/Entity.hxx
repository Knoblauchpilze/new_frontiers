#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"

namespace new_frontiers {

  inline
  Entity::Entity(const EntityTile& desc,
                 const Effect& vfx):
    WorldElement(desc, "entity"),

    m_ox(desc.x),
    m_oy(desc.y),

    m_started(false),
    m_radius(1.0f),
    m_lap(toMilliseconds(2000)),
    m_init(now()),

    m_vfx(vfx),

    m_vfxDelta(toMilliseconds(1000)),
    m_last(now() - m_vfxDelta)
  {}

  inline
  bool
  Entity::step(StepInfo& info) {
    // Make the entity walk on a circle with
    // random speed and radius.
    if (!m_started) {
      m_started = true;

      m_radius = info.rng.rndFloat(0.0f, 3.0f);
      m_lap = toMilliseconds(info.rng.rndInt(2000, 4000));
    }

    Duration elapsed = info.moment - m_init;
    float period = 1.0f * elapsed.count() / m_lap.count();

    m_tile.x = m_ox - m_radius + m_radius * std::cos(2.0f * 3.14159f * period);
    m_tile.y = m_oy            + m_radius * std::sin(2.0f * 3.14159f * period);

    info.clampCoord(m_tile.x, m_tile.y);

    // Check whether we shoud spawn a new vfx.
    if (info.moment - m_vfxDelta < m_last) {
      return true;
    }

    // Now is the last time we generated a vfx
    // for this entity.
    m_last = info.moment + m_vfxDelta;

    // Generate a new vfx at the location of
    // the entity.
    VFXTile v;

    v.type = m_vfx;
    v.id = 0;
    v.x = m_tile.x;
    v.y = m_tile.y;

    info.vSpawned.push_back(std::make_shared<VFX>(v));

    return true;
  }

}

#endif    /* ENTITY_HXX */

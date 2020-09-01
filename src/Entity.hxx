#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"

namespace new_frontiers {

  inline
  void
  PathSegment::animate(const TimeStamp& moment, float& x, float& y) const {
    // Compute the ratio between this moment and
    // the total time and use this as a percentage
    // of completion for the path.
    float p = 1.0f;

    if (start != end) {
      Duration d = moment - start;
      Duration t = end - start;

      p = std::min(std::max(1.0f * d.count() / t.count(), 0.0f), 1.0f);
    }

    x = (1.0f - p) * xO + p * xT;
    y = (1.0f - p) * yO + p * yT;
  }

  inline
  float
  PathSegment::length() const noexcept {
    return std::sqrt((xT - xO) * (xT - xO) + (yT - yO) * (yT - yO));
  }

  inline
  Entity::Entity(const EntityTile& desc,
                 const Effect& vfx,
                 int vfxID,
                 bool decaying):
    WorldElement(desc, "entity"),

    m_vfx(vfx),
    m_vfxID(vfxID),
    m_vfxDecay(decaying),

    m_speed(-1.0f),
    m_rArrival(0.2f),
    m_pathLength(3.0f),

    m_hasPath(false),
    m_path(),

    m_state{
      false, // Glowing.
      false  // Exhausted.
    }
  {}

  inline
  const State&
  Entity::getState() const noexcept {
    return m_state;
  }

  inline
  void
  Entity::makeGlow(bool glowing) noexcept {
    m_state.glowing = glowing;
  }

  inline
  void
  Entity::makeExhausted(bool exhausted) noexcept {
    m_state.exhausted = exhausted;
  }

  inline
  VFXShPtr
  Entity::spawnVFX() const noexcept {
    VFXTile vt = newTile(m_vfx, m_vfxID);

    vt.x = m_tile.x;
    vt.y = m_tile.y;

    return std::make_shared<VFX>(vt, m_vfxDecay);
  }

}

#endif    /* ENTITY_HXX */

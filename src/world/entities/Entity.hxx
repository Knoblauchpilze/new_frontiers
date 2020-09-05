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
    Element(desc, "entity"),

    m_vfx(vfx),
    m_vfxID(vfxID),
    m_vfxDecay(decaying),

    m_speed(-1.0f),
    m_rArrival(0.05f),
    m_pathLength(3.0f),

    m_hasPath(false),
    m_path(),

    m_state{
      false, // Glowing.
      false  // Exhausted.
    },

    m_passed(),

    m_cPoints()
  {
    // Assign the path with initial position
    // of the entity.
    m_path.xO = m_tile.x;
    m_path.yO = m_tile.y;

    m_path.xT = m_tile.x;
    m_path.yT = m_tile.y;

    m_path.start = now();
    m_path.end = now();
  }

  inline
  void
  Entity::pause(const TimeStamp& t) {
    // We need to make sure that the path can be
    // continued in good conditions after pausing
    // the simulation. This means that we want to
    // restore the percentage of progression as
    // it is at the moment of the pause.
    // Of course it only applies in case we have
    // a path.
    m_passed = Duration::zero();

    if (m_hasPath) {
      m_passed = t - m_path.start;
    }
  }

  inline
  void
  Entity::resume(const TimeStamp& t) {
    // In case there's something to resume.
    if (m_passed != Duration::zero()) {
      m_path.start = t - m_passed;
      m_path.end = m_path.start + toMilliseconds(static_cast<int>(1000.0f * m_path.length() / m_speed));
    }
  }

  inline
  const State&
  Entity::getState() const noexcept {
    return m_state;
  }

  inline
  float
  Entity::getPathX() const noexcept {
    return (m_hasPath ? m_path.xT : m_tile.x);
  }

  inline
  float
  Entity::getPathY() const noexcept {
    return (m_hasPath ? m_path.yT : m_tile.y);
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

  inline
  void
  Entity::normalizePath(const StepInfo& info, float& xDir, float& yDir, float& d) const noexcept {
    // Compute the final position of the path.
    float xt = m_tile.x + d * xDir;
    float yt = m_tile.y + d * yDir;

    // Clamp coordinates.
    info.clampCoord(xt, yt);

    // Update the direction.
    xDir = xt - m_tile.x;
    yDir = yt - m_tile.y;

    // Update the distance.
    d = std::sqrt(xDir * xDir + yDir * yDir);

    if (d > 0.0001f) {
      xDir /= d;
      yDir /= d;
    }
  }

}

#endif    /* ENTITY_HXX */

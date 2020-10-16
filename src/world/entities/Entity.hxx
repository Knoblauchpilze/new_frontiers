#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"
# include "../effects/PheromonFactory.hh"

namespace new_frontiers {

  inline
  float
  PathSegment::length() const noexcept {
    return std::sqrt((xT - xO) * (xT - xO) + (yT - yO) * (yT - yO));
  }

  inline
  const State&
  Entity::getState() const noexcept {
    return m_state;
  }

  inline
  float
  Entity::getPerceptionRadius() const noexcept {
    return m_perceptionRadius;
  }

  inline
  float
  Entity::getPathX() const noexcept {
    return (isEnRoute() ? m_path.xT : m_tile.x);
  }

  inline
  float
  Entity::getPathY() const noexcept {
    return (isEnRoute() ? m_path.yT : m_tile.y);
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
  PheromonShPtr
  Entity::spawnPheromon(const pheromon::Type& type) const noexcept {
    Pheromon::PProps pp = PheromonFactory::newPheromonProps(
      m_tile.x,
      m_tile.y,
      PheromonFactory::pheromonToVFX(type)
    );

    pp.radius = getRadius();
    pp.type = type;

    pp.owner = getOwner();

    return PheromonFactory::newPheromon(pp);
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

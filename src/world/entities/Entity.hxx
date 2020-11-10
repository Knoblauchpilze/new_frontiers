#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"
# include "../effects/PheromonFactory.hh"

namespace new_frontiers {

  inline
  Entity::Props::~Props() {}

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
  path::Path
  Entity::getPath() const noexcept {
    return m_path;
  }

  inline
  bool
  Entity::isEnRoute() const noexcept {
    return m_path.enRoute(m_rArrival);
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
    Pheromon::PProps pp = PheromonFactory::newPheromonProps(m_tile.p.x, m_tile.p.y, type);

    pp.radius = getRadius();
    pp.type = type;

    pp.owner = getOwner();

    return PheromonFactory::newPheromon(pp);
  }

}

#endif    /* ENTITY_HXX */

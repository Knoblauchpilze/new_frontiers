#ifndef    ELEMENT_HXX
# define   ELEMENT_HXX

# include "Element.hh"

namespace new_frontiers {

  template <typename TileType>
  inline
  const Tile<TileType>&
  Element<TileType>::getTile() const noexcept {
    return m_tile;
  }

  template <typename TileType>
  inline
  Tile<TileType>&
  Element<TileType>::getTile() noexcept {
    return m_tile;
  }

  template <typename TileType>
  inline
  float
  Element<TileType>::getRadius() const noexcept {
    return m_radius;
  }

  template <typename TileType>
  inline
  float
  Element<TileType>::getHealthRatio() {
    if (m_totalHealth <= 0.0f) {
      return 1.0f;
    }

    return m_health / m_totalHealth;
  }

  template <typename TileType>
  inline
  bool
  Element<TileType>::isDead() const noexcept {
    return m_toBeDeleted;
  }

  template <typename TileType>
  bool
  Element<TileType>::damage(float hit) {
    // In case the `hit` is in fact a healing
    // we can do that in the limit of the health
    // pool for this element.
    if (hit < 0.0f) {
      m_health = std::min(m_health - hit, m_totalHealth);

      return true;
    }

    // Otherwise, clamp to a null health.
    m_health = std::max(m_health - hit, 0.0f);

    return (m_health > 0.0f);
  }

  template <typename TileType>
  inline
  Element<TileType>::Element(const Tile<TileType>& desc,
                             float radius,
                             float health,
                             const std::string& name):
    utils::CoreObject(name),

    m_tile(desc),

    // Make sure to set the radius to a valid value.
    m_radius(radius < 0.0f ? 1.0f : radius),
    // Clamp to at least `0`.
    m_health(std::max(health, 0.0f)),
    m_totalHealth(m_health),

    m_toBeDeleted(false)
  {
    setService("element");
  }

  template <typename TileType>
  inline
  void
  Element<TileType>::markForDeletion(bool toDelete) {
    m_toBeDeleted = toDelete;
  }

}

#endif    /* ELEMENT_HXX */

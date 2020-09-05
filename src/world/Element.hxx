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
  Element<TileType>::Element(const Tile<TileType>& desc,
                             const std::string& name):
    utils::CoreObject(name),

    m_tile(desc)
  {
    setService("element");
  }

}

#endif    /* ELEMENT_HXX */

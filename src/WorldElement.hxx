#ifndef    WORLD_ELEMENT_HXX
# define   WORLD_ELEMENT_HXX

# include "WorldElement.hh"


namespace new_frontiers {

  template <typename TileType>
  inline
  const Tile<TileType>&
  WorldElement<TileType>::getTile() const noexcept {
    return m_tile;
  }

  template <typename TileType>
  inline
  Tile<TileType>&
  WorldElement<TileType>::getTile() noexcept {
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

}

#endif    /* WORLD_ELEMENT_HXX */

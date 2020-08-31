#ifndef    WORLD_ELEMENT_HH
# define   WORLD_ELEMENT_HH

# include <core_utils/CoreObject.hh>
# include "WorldTypes.hh"

namespace new_frontiers {

  template <typename TileType>
  class WorldElement: public utils::CoreObject {
    public:

      /**
       * @brief - Gets the tile associated with this element.
       * @return - the tile for this object.
       */
      const Tile<TileType>&
      getDesc() const noexcept;

      /**
       * @brief - Non constant variant of the above method.
       * @return - the tile associated to this object.
       */
      Tile<TileType>&
      getDesc() noexcept;

      /**
       * @brief - Interface method allowing for a world element
       *          to evolve based on its surroundings. We use a
       *          struct gathering the current state of the
       *          world to take decisions based on it.
       *          Inheriting classes can subclass this method
       *          to provide custom behavior.
       * @param info - all the information about the current
       *               state of the world.
       * @return - `true` if the element has been updated. It
       *           is used to indicate that a repaint of the
       *           entity (and most probably of the wolrd) is
       *           needed.
       */
      virtual bool
      step(StepInfo& info) = 0;

    protected:

      /**
       * @brief - Build a new world element with the specified
       *          tile and name.
       * @param tile - the tile to associate to this object. It
       *               defines the visual along with the coords
       *               of the element.
       * @param name - the name of the object (mainly used for
       *               logging purposes).
       */
      WorldElement(const Tile<TileType>& tile,
                   const std::string& name);

    protected:

      /**
       * @brief - The visual representation of this element and
       *          the coordinates at which it should be displayed.
       */
      Tile<TileType> m_tile;
  };

}

# include "WorldElement.hxx"

#endif    /* WORLD_ELEMENT_HH */

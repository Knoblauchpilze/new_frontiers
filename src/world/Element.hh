#ifndef    ELEMENT_HH
# define   ELEMENT_HH

# include <core_utils/CoreObject.hh>
# include "Tiles.hh"
# include "TimeUtils.hh"
# include "StepInfo.hh"

namespace new_frontiers {

  template <typename TileType>
  class Element: public utils::CoreObject {
    public:

      /**
       * @brief - Gets the tile associated with this element.
       * @return - the tile for this object.
       */
      const Tile<TileType>&
      getTile() const noexcept;

      /**
       * @brief - Non constant variant of the above method.
       * @return - the tile associated to this object.
       */
      Tile<TileType>&
      getTile() noexcept;

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

      /**
       * @brief - Interface method allowing to pause this world
       *          element: all internal behavior that depend on
       *          time should be stopped so that they can be
       *          later resumed.
       * @param t - the timestamp at which the pause occur.
       */
      virtual void
      pause(const TimeStamp& t) = 0;

      /**
       * @brief - Opposite operation to the `pause`, this method
       *          is called to indicate that internal processes
       *          for this world element can be resumed.
       * @param t - the timestamp at which the resume occur.
       */
      virtual void
      resume(const TimeStamp& t) = 0;

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
      Element(const Tile<TileType>& tile,
              const std::string& name);

    protected:

      /**
       * @brief - The visual representation of this element and
       *          the coordinates at which it should be displayed.
       */
      Tile<TileType> m_tile;
  };

}

# include "Element.hxx"

#endif    /* ELEMENT_HH */

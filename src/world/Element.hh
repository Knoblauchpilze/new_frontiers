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
       * @brief - Return the radius for this entity.
       * @return - the radius for this entity in blocks.
       */
      float
      getRadius() const noexcept;

      /**
       * @brief - Retrieve the ratio of current health over
       *          total health for this element. This value
       *          is in the range `[0; 1]` and allows to
       *          measure how healthy this element it.
       * @return - a ratio measuring the health of this item.
       */
      float
      getHealthRatio();

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
       * @param radius - the radius for this entity expressed
       *                 in blocks.
       * @param health - the health pool for this element.
       * @param name - the name of the object (mainly used for
       *               logging purposes).
       */
      Element(const Tile<TileType>& tile,
              float radius,
              float health,
              const std::string& name);

    protected:

      /**
       * @brief - The visual representation of this element and
       *          the coordinates at which it should be displayed.
       */
      Tile<TileType> m_tile;

      /**
       * @brief - Define the size of the entity expressed in
       *          blocks. This value is guaranteed to be at
       *          least `0`.
       */
      float m_radius;

      /**
       * @brief - The health pool for this element.
       */
      float m_health;

      /**
       * @brief - The total health pool for this element. It is
       *          used as a reminder of how much health in total
       *          the element had, before anything happened to
       *          it.
       */
      float m_totalHealth;
  };

}

# include "Element.hxx"

#endif    /* ELEMENT_HH */

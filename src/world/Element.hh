#ifndef    ELEMENT_HH
# define   ELEMENT_HH

# include <core_utils/CoreObject.hh>
# include <core_utils/Uuid.hh>
# include "Tiles.hh"
# include <core_utils/TimeUtils.hh>
# include "StepInfo.hh"
# include "WorldElement.hh"

namespace new_frontiers {

  class StepInfo;

  template <typename TileType>
  class Element: public WorldElement {
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
       * @brief - Return `true` in case this element is marked
       *          for deletion and will probably be removed in
       *          the next execution of the influences.
       * @return - `true` if the block is marked for deletion.
       */
      bool
      isDead() const noexcept;

      /**
       * @brief - Used to hit the element with the specified
       *          amount of damage. We clamp the amount of
       *          health to `0` and the return value tells
       *          whether or not the element is still alive.
       *          Note that in case the `hit` is negative
       *          it will correspond to some healing done
       *          to the element.
       * @param hit - the amount of damage to apply to the
       *              element.
       */
      bool
      damage(float hit);

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
       * @param owner - the identifier of the owner of this
       *                element. It is assigned to a null value
       *                by default, meaning that the element is
       *                not owned.
       */
      Element(const Tile<TileType>& tile,
              float radius,
              float health,
              const std::string& name,
              const utils::Uuid& owner = utils::Uuid());

      /**
       * @brief - Used to mark or remove this element from
       *          deletion.
       * @param toDelete - `true` if the element should be
       *                   marked for deletion.
       */
      void
      markForDeletion(bool toDelete);

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

      /**
       * @brief - Used to indicate that this element is marked
       *          for deletion. Interesting to allow inheriting
       *          classes to determine whether this block is
       *          still relevant.
       */
      bool m_toBeDeleted;
  };

}

# include "Element.hxx"

#endif    /* ELEMENT_HH */

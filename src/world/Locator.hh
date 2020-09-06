#ifndef    LOCATOR_HH
# define   LOCATOR_HH

# include <memory>
# include <unordered_set>
# include <core_utils/CoreObject.hh>
# include "blocks/Block.hh"
# include "entities/Entity.hh"
# include "effects/VFX.hh"

namespace new_frontiers {

  // Forward declaration of types used by the locator.
  class Block;
  class Entity;
  class VFX;

  using BlockShPtr = std::shared_ptr<Block>;
  using EntityShPtr = std::shared_ptr<Entity>;
  using VFXShPtr = std::shared_ptr<VFX>;

  /**
   * @brief - Indicates the type of update to perform on this
   *          elements locator. A `Full` update usually says
   *          that some elements have either be added or have
   *          been deleted meaning that the structure might
   *          be better off completely rebuilt. A `Light`
   *          indicates that only motions have been performed.
   *          It is merely an indication to the locator so
   *          that it can perform the most suited update.
   */
  enum class Update {
    Full,
    Light
  };

  class Locator: public utils::CoreObject {
    public:

      /**
       * @brief - Build a new locator for in-game elements. This
       *          class aims at providing efficient location and
       *          filtering method to detect specific entities
       *          or solid elements in the world.
       *          The details of the implementation do not need
       *          to be exposed to external users so it's hidden
       *          in here. One can use the methods to query the
       *          elements through the dedicated interface.
       * @param width - the width of the world in which elements
       *                are registered. Expressed in cells.
       * @param height - similar to `width` but for the height
       *                 of the world.
       * @param blocks - the list of tiles registered in the
       *                 world.
       * @param entities - the list of entities of the world.
       * @param vfx - the list of visual effects of the world.
       */
      Locator(int width,
              int height,
              const std::vector<BlockShPtr>& blocks,
              const std::vector<EntityShPtr>& entities,
              const std::vector<VFXShPtr>& vfx);

      /**
       * @brief - Used to indicate that the locator should be
       *          recomputing the spatially-optimized layout
       *          due probably to some motions.
       *          An internal system is used to detect items
       *          that have changed so it's somewhat safe to
       *          call this more often than needed.
       * @param update - the type of update to perform. It is
       *                 indicating the type of modifications
       *                 that have been performed on tiles or
       *                 entities compared to when the struct
       *                 of this object was built.
       */
      void
      refresh(const Update& update);

      /**
       * @brief - Used to count how many entities of the input
       *          type exists within the region defined by the
       *          coordinates and radius.
       * @param ent - the type of entity to search.
       * @param x - the center of the radius in which entities
       *            should be searched.
       * @param y - second coordinate of the center.
       * @param radius - the radius of the center to search in.
       * @return - an integer count representing the number of
       *           entities of said type in the input region.
       */
      int
      countEntities(const tiles::Entity& ent, float x, float y, float radius) const;

      /**
       * @brief - Used to compute the distance between point
       *          `(x1, y1)` and `(x2, y2)`.
       * @param x1 - abscissa of the first point.
       * @param y1 - ordinate of the first point.
       * @param x2 - abscissa of the second point.
       * @param y2 - ordinate of the second point.
       * @return - the distance between the two points.
       */
      float
      d(float x1, float y1, float x2, float y2) const noexcept;

      /**
       * @brief - Similar to `d` but returns the squared
       *          distance between two points.
       * @param x1 - abscissa of the first point.
       * @param y1 - ordinate of the first point.
       * @param x2 - abscissa of the second point.
       * @param y2 - ordinate of the second point.
       * @return - the squared distance between the two
       *           input points.
       */
      float
      d2(float x1, float y1, float x2, float y2) const noexcept;

      /**
       * @brief - Determine whether the path defined by the
       *          input coordinate and the direction has any
       *          obstructing elements on it within `d` cells
       *          of the starting position.
       *          Note that we don't consider the first cell
       *          to be a valid candidate for obstruction.
       * @param x - the starting abscissa of the path.
       * @param y - the ordinate of the path.
       * @param xDir - the direction of the ray along the `x`
       *               axis.
       * @param yDir - the direction of the ray along the `y`
       *               axis.
       * @param d - the distance to check for obstruction.
       * @return - `true` if the path is obstructed by any
       *           solid element.
       */
      bool
      obstructed(float x, float y, float xDir, float yDir, float d, std::vector<float>& cPoints) const noexcept;

    private:

      /**
       * @brief - Used to initialize the internal variables to
       *          compute information about the data stored in
       *          this world.
       */
      void
      initialize();

    private:

      /**
       * @brief - The width of the world in cells.
       */
      int m_w;

      /**
       * @brief - The height of the world in cells.
       */
      int m_h;

      /**
       * @brief - The elements registered in the world.
       */
      const std::vector<BlockShPtr>& m_blocks;
      const std::vector<EntityShPtr>& m_entities;
      const std::vector<VFXShPtr>& m_vfx;

      /**
       * @brief - A map referencing the unique indices for
       *          blocks in the world. It is build as the
       *          object is constructed (or refresehd) and
       *          allows to quickly access to the id where
       *          a block exists.
       */
      std::unordered_set<int> m_blocksIDs;
  };

  using LocatorShPtr = std::shared_ptr<Locator>;
}

# include "Locator.hxx"

#endif    /* LOCATOR_HH */

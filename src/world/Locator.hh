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

  /**
   * @brief - Convenience wrapper describing common props
   *          for blocks that can be used for display.
   */
  struct BlockDesc {
    BlockTile tile;
    float health;
    float ratio;
  };

  /**
   * @brief - Convenience wrapper to retrieve information
   *          about an entity. This is passed as return
   *          value for elements querying entities from
   *          this iterator in order to have all public
   *          info of an entity avaiable.
   */
  struct EntityDesc {
    EntityTile tile;
    float radius;
    float percepRadius;
    float health;
    float carrying;
    float cargo;
    State state;
    float xT;
    float yT;
    std::vector<float> cPoints;
  };

  /**
   * @brief - Similar to the `EntityDesc` but for effects.
   *          Contains an indication on the amount of the
   *          initial quantity of the effect that is still
   *          existing.
   *          The amount is always in the range `[0; 1]`.
   */
  struct VFXDesc {
    VFXTile tile;
    float radius;
    float amount;
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
       * @brief - Return the width of the world in cells.
       * @return - the number of cell in width for the
       *           world attached to this locator.
       */
      int
      w() const noexcept;

      /**
       * @brief - Return the height of the world in cells.
       * @return - the number of cell in height for the
       *           world attached to this locator.
       */
      int
      h() const noexcept;

      /**
       * @brief - Retrieve the tile at the specified index. Note
       *          that no checks are performed to verify that it
       *          is indeed possible to access to this tile: if
       *          it is not UB will arise.
       * @param id - index of the tile to access.
       * @return - the tile at the specified index.
       */
      BlockDesc
      block(int id) const noexcept;

      /**
       * @brief - Similar to the `solidTile` method but to get
       *          the entity at the specified index.
       * @param id - the index of the entity to get.
       * @return - the corresponding entity.
       */
      EntityDesc
      entity(int id) const noexcept;

      /**
       * @brief - Very similar but for VFX.
       * @param id - the index of the VFX to get.
       * @return - the corresponding VFX.
       */
      VFXDesc
      vfx(int id) const noexcept;

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
      obstructed(float x,
                 float y,
                 float xDir,
                 float yDir,
                 float d,
                 std::vector<float>& cPoints) const noexcept;

      /**
       * @brief - Retrieve the list of elements with a type
       *          identical to the one provided in input. A
       *          definition of a circle (through a position
       *          and an area) is also provided.
       * @param x - the abscissa of the center of the area
       *            to search for.
       * @param y - the ordinate of the center of the area
       *            to search for.
       * @param r - the radius of the area to search for. If
       *            this value is negative there's no limits
       *            to the distance between the block and the
       *            input position.
       * @param block - the type of element to search for.
       * @param id - the variant of the block to search for.
       *             If a negative value is specified all
       *             variants will be considered valid.
       * @param sort - `true` if the list of blocks should be
       *               sorted from closest to farthest.
       * @return - the list of blocks corresponding in the
       *           specified area.
       */
      std::vector<BlockShPtr>
      getBlocks(float x,
                float y,
                float r,
                const tiles::Block& block,
                int id = 0,
                bool sort = false) const noexcept;

      /**
       * @brief - Similar to the `getBlocks` method but
       *          only returns the closest block from
       *          the list returned.
       *          In case there are no blocks existing
       *          the return value will be `null`.
       * @param x - the abscissa of the position from
       *            which the closest block should be
       *            found.
       * @param y - the ordinate of the position from
       *            which the closest block should be
       *            found.
       * @param block - the block to search for.
       * @param id - the variant of the block to search
       *             for, or `-1` in case the variant
       *             does not matter.
       * @return - the corresponding block or `null` if
       *           none can be found.
       */
      BlockShPtr
      getClosest(float x,
                 float y,
                 const tiles::Block& block,
                 int id = 0) const noexcept;

      /**
       * @brief - Retrieve the list of entities with a type
       *          identical to the one provided in input. A
       *          definition of a circle (through a position
       *          and an area) is also provided.
       * @param x - the abscissa of the center of the area
       *            to search for.
       * @param y - the ordinate of the center of the area
       *            to search for.
       * @param r - the radius of the area to search for. If
       *            this value is negative there's no limits
       *            to the distance between the block and the
       *            input position.
       * @param ent - the type of entity to search for.
       * @param id - the variant of the entity to search for.
       *             If a negative value is specified all
       *             variants will be considered valid.
       * @param sort - `true` if the list of entities should
       *               be sorted from closest to farthest.
       * @return - the list of entities corresponding in the
       *           specified area.
       */
      std::vector<EntityShPtr>
      getEntities(float x,
                  float y,
                  float r,
                  const tiles::Entity& ent,
                  int id = 0,
                  bool sort = false) const noexcept;

      /**
       * @brief - Similar to the `getEntities` method but
       *          only returns the closest entity from
       *          the list returned.
       *          In case there are no entities existing
       *          the return value will be `null`.
       * @param x - the abscissa of the position from
       *            which the closest entity should be
       *            found.
       * @param y - the ordinate of the position from
       *            which the closest entity should be
       *            found.
       * @param ent - the entity to search for.
       * @param id - the variant of the entity to search
       *             for, or `-1` in case the variant
       *             does not matter.
       * @return - the corresponding entity or `null` if
       *           none can be found.
       */
      EntityShPtr
      getClosest(float x,
                 float y,
                 const tiles::Entity& ent,
                 int id = 0) const noexcept;

    private:

      /**
       * @brief - Used to initialize the internal variables to
       *          compute information about the data stored in
       *          this world.
       */
      void
      initialize();

      /**
       * @brief - Used to perform the sorting of tiles, entities
       *          and vfx. Indeed to make sure that all tiles are
       *          displayed in order and are correctly hidden by
       *          tiles that are in front of them, we need to
       *          sort the tiles by increasing `x` value and by
       *          increasing `y` value.
       */
      void
      sort();

    private:

      /**
       * @brief - Define a convenience structure to perform the
       *          sorting of tiles and entities.
       */
      struct SortEntry {
        float x;
        float y;
        int id;
      };

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

      std::vector<SortEntry> m_sortedBlocks;
      std::vector<SortEntry> m_sortedEntities;
      std::vector<SortEntry> m_sortedVFX;
  };

  using LocatorShPtr = std::shared_ptr<Locator>;
}

# include "Locator.hxx"

#endif    /* LOCATOR_HH */

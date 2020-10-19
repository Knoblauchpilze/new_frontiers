#ifndef    LOCATOR_HH
# define   LOCATOR_HH

# include <memory>
# include <unordered_set>
# include <core_utils/CoreObject.hh>
# include "blocks/Block.hh"
# include "entities/Entity.hh"
# include "effects/VFX.hh"
# include "colonies/Colony.hh"

namespace new_frontiers {

  // Forward declaration of types used by the locator.
  class Block;
  class Entity;
  class VFX;

  using BlockShPtr = std::shared_ptr<Block>;
  using EntityShPtr = std::shared_ptr<Entity>;
  using VFXShPtr = std::shared_ptr<VFX>;

  namespace world {

    /**
     * @brief - Convenience structure allowing to define
     *          a status for identifiers when retrieving
     *          elements based on their visibility.
     */
    struct Filter {
      utils::Uuid id;
      bool include;
    };

    /**
     * @brief - Convenience wrapper describing common props
     *          for blocks that can be used for display.
     */
    struct Block {
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
    struct Entity {
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
     * @brief - Similar to the `Entity` but for effects.
     *          Contains an indication on the amount of the
     *          initial quantity of the effect that is still
     *          existing.
     *          The amount is always in the range `[0; 1]`.
     */
    struct VFX {
      VFXTile tile;
      float radius;
      float amount;
    };

    /**
     * @brief - Convenience wrapper to define the information
     *          about a colony.
     */
    struct Colony {
      utils::Uuid id;
      colony::Priority focus;
      float ratio;
    };

    /**
     * @brief - Convenience enumeration defining the possible
     *          types for an item visible in the frustum of
     *          the locator.
     */
    enum class ItemType {
      Block,
      Entity,
      VFX
    };

    /**
     * @brief - Convenience structure defining an entry in the
     *          visibility frustum returned by the locator. It
     *          defines the type of the object along with its
     *          index which can then be used to fetch it from
     *          the locator itself (and thus retrieve more info
     *          on it).
     */
    struct ItemEntry {
      ItemType type;
      int index;
    };

    /**
     * @brief - Transform the input focus into a readable string.
     *          In case the focus cannot be interpreted "unknown"
     *          is returned.
     * @param focus - the focus to interpret as a string.
     * @return - the corresponding string for the input focus.
     */
    std::string
    focusToString(const colony::Priority& focus) noexcept;

  }

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
       * @param colonies - the list of colonies registered in
       *                   this world.
       * @param vfxs - the list of visual effects of the world.
       */
      Locator(int width,
              int height,
              const std::vector<BlockShPtr>& blocks,
              const std::vector<EntityShPtr>& entities,
              const std::vector<VFXShPtr>& vfxs,
              const std::vector<ColonyShPtr>& colonies);

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
      world::Block
      block(int id) const noexcept;

      /**
       * @brief - Similar to the `solidTile` method but to get
       *          the entity at the specified index.
       * @param id - the index of the entity to get.
       * @return - the corresponding entity.
       */
      world::Entity
      entity(int id) const noexcept;

      /**
       * @brief - Very similar but for VFX.
       * @param id - the index of the VFX to get.
       * @return - the corresponding VFX.
       */
      world::VFX
      vfx(int id) const noexcept;

      /**
       * @brief - Return the number of colonies registered so
       *          far in the world.
       * @return - the number of colonies in this world.
       */
      int
      coloniesCount() const noexcept;

      /**
       * @brief - Return the colony at index `id`
       * @param id - the index of the colony to fetch.
       * @return - a description of the colony to get.
       */
      world::Colony
      colony(int id) const noexcept;

      /**
       * @brief - Used to indicate that the locator should be
       *          recomputing the spatially-optimized layout
       *          due probably to some motions.
       *          An internal system is used to detect items
       *          that have changed so it's somewhat safe to
       *          call this more often than needed.
       */
      void
      refresh();

      /**
       * @brief - Allow to determine whether a specific loc
       *          is obstructed (meaning that a block already
       *          exists here) or not.
       * @param x - the abscissa to check for occupancy.
       * @param y - the ordinate to check for occupancy.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(float x, float y) const noexcept;

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
       * @brief - Return the list of items that are visible
       *          in the view frustum defined by the AABB
       *          from the input values.
       *          All items are included (blocks, entities
       *          and VFXs) and the user can request items
       *          to be sorted.
       * @param xMin - the abscissa of the top left corner
       *               of the view frustum.
       * @param yMin - the ordinate of the top left corner
       *               of the view frustum.
       * @param xMax - the abscissa of the bottom right
       *               corner of the view frustum.
       * @param yMax - the ordinate of the bottom right
       *               corner of the view frustum.
       * @param type - the type of elements to consider. If
       *               this value is set to `null` all the
       *               item types will be included.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @param sort - `true` if the items should be sorted
       *               in the output vector. The sort occurs
       *               based on the `z` order of the items.
       * @return - the list of items that are visible in the
       *           input view frustum.
       */
      std::vector<world::ItemEntry>
      getVisible(float xMin,
                 float yMin,
                 float xMax,
                 float yMax,
                 const world::ItemType* type = nullptr,
                 const world::Filter* filters = nullptr,
                 bool sort = false) const noexcept;

      /**
       * @brief - Retrieve the list of elements with a type
       *          as defined in input or all the elements if
       *          no type is provided.
       *          The area in which elements should be fetched
       *          is described through a circle with a center
       *          and a radius.
       * @param x - the abscissa of the center of the area to
       *            search for.
       * @param y - the ordinate of the center of the area to
       *            search for.
       * @param r - the radius of the area to search for. If
       *            this value is negative there's no limits
       *            to the distance between the element and the
       *            input position.
       * @param type - the type of elements to consider. If
       *               this value is set to `null` all the
       *               item types will be included.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @param sort - `true` if the items should be sorted
       *               in the output vector. The sort occurs
       *               based on the `z` order of the items.
       * @return - the list of elements corresponding in the
       *           specified area.
       */
      std::vector<world::ItemEntry>
      getVisible(float x,
                 float y,
                 float r,
                 const world::ItemType* type = nullptr,
                 const world::Filter* filter = nullptr,
                 bool sort = false) const noexcept;

      /**
       * @brief - Similar to the `getVisible` method but
       *          only returns the closest element from
       *          the available items.
       *          In case there are no elements to return
       *          the output will have a negative index.
       * @param x - the abscissa of the position from
       *            which the closest block should be
       *            found.
       * @param y - the ordinate of the position from
       *            which the closest block should be
       *            found.
       * @param type - the type of the element to search
       *               for.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @return - the corresponding element's description
       *           or a negative index if none can be
       *           found.
       */
      world::ItemEntry
      getClosest(float x,
                 float y,
                 const world::ItemType& type,
                 const world::Filter& filter) const noexcept;

      /**
       * @brief - Specialization of the `getVisible` method
       *          to fetch visible blocks in a certain area
       *          and return this as a list of blocks.
       * @param x - the abscissa of the center of the area
       *            to consider.
       * @param y - the ordinate of the center of the area
       *            to consider.
       * @param r - the radius of the area to consider.
       * @param bTile - the type of the blocks to consider.
       *                If this value is `null` any block
       *                will be considered.
       * @param id - the variant of the block to consider.
       *             If this value is `-1` any variant is
       *             considered.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @param sort - whether or not the output vector
       *               should be sorted so that blocks are
       *               in ascending order compared to the
       *               center of the area.
       * @return - the list of blocks.
       */
      std::vector<BlockShPtr>
      getVisible(float x,
                 float y,
                 float r,
                 const tiles::Block* bTile,
                 int id = -1,
                 const world::Filter* filter = nullptr,
                 bool sort = false) const noexcept;

      /**
       * @brief - Specialization of the `getVisible` method
       *          to fetch visible entities in a certain area
       *          and return this as a list of entities.
       * @param x - the abscissa of the center of the area
       *            to consider.
       * @param y - the ordinate of the center of the area
       *            to consider.
       * @param r - the radius of the area to consider.
       * @param bTile - the type of the entity to consider.
       *                If this value is `null` any entity
       *                will be considered.
       * @param id - the variant of the entity to consider.
       *             If this value is `-1` any variant is
       *             considered.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @param sort - whether or not the output vector
       *               should be sorted so that entities
       *               are in ascending order compared to
       *               the center of the area.
       * @return - the list of entities.
       */
      std::vector<EntityShPtr>
      getVisible(float x,
                 float y,
                 float r,
                 const tiles::Entity* eTile,
                 int id = -1,
                 const world::Filter* filter = nullptr,
                 bool sort = false) const noexcept;

      /**
       * @brief - Similar to the `getVisible` but only returns
       *          the closest block from the total visible list.
       * @param x - the abscissa of the center of the area to
       *            consider.
       * @param y - the ordinate of the center of the area to
       *            consider.
       * @param bTile - the type of the blocks to consider. If
       *                this value is `null` any block will be
       *                considered.
       * @param r - a limit for the radius for entities to be
       *            considered.
       * @param id - the variant of the block to consider. If
       *             this value is `-1` any variant is considered.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @return - the list of blocks.
       */
      BlockShPtr
      getClosest(float x,
                 float y,
                 const tiles::Block& bTile,
                 float r = -1.0f,
                 int id = -1,
                 const world::Filter* filter = nullptr) const noexcept;

      /**
       * @brief - Similar to the `getVisible` but only returns
       *          the closest entity from the total visible list.
       * @param x - the abscissa of the center of the area to
       *            consider.
       * @param y - the ordinate of the center of the area to
       *            consider.
       * @param bTile - the type of the entities to consider. If
       *                this value is `null` any entity will be
       *                considered.
       * @param r - a limit for the radius for entities to be
       *            considered.
       * @param id - the variant of the entity to consider. If
       *             this value is `-1` any variant is considered.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @return - the list of entities.
       */
      EntityShPtr
      getClosest(float x,
                 float y,
                 const tiles::Entity& eTile,
                 float r = -1.0f,
                 int id = -1,
                 const world::Filter* filter = nullptr) const noexcept;

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
       * @brief - Define a convenience structure to perform the
       *          sorting of tiles and entities.
       */
      struct SortEntry {
        float x;
        float y;
        unsigned id;
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
      const std::vector<VFXShPtr>& m_vfxs;

      /**
       * @brief - The list of colonies for this world.
       */
      const std::vector<ColonyShPtr>& m_colonies;

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

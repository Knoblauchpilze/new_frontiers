#ifndef    ITERATORS_HH
# define   ITERATORS_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "Tiles.hh"
# include "blocks/Block.hh"
# include "entities/Entity.hh"
# include "effects/VFX.hh"

namespace new_frontiers {

  /**
   * @brief - Convenience wrapper describing common props
   *          for blocks that can be used for display.
   */
  struct BlockDesc {
    BlockTile tile;
    float health;
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

  class Iterator: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new iterator on world's objects.
       * @param width - the width of the world in cells.
       * @param height - the height of the world in cells.
       * @param blocks - the list of blocks for this world.
       * @param entities - the list of entities in the world.
       * @param vfx - the list of vfx for the world.
       */
      Iterator(int width,
               int height,
               const std::vector<BlockShPtr>& blocks,
               const std::vector<EntityShPtr>& entities,
               const std::vector<VFXShPtr>& vfx);

      ~Iterator() = default;

      int
      w() const noexcept;

      int
      h() const noexcept;

      int
      blocksCount() const noexcept;

      int
      entitiesCount() const noexcept;

      int
      vfxCount() const noexcept;

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
       * @brief - Called whenever the tiles, entities or vfx have
       *          been updated (for example with the generation of
       *          new elements). In this case the iterator needs
       *          to re-order the items to still enable correct
       *          display.
       */
      void
      refresh();

    private:

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

      int m_w;
      int m_h;

      const std::vector<BlockShPtr>& m_blocks;
      const std::vector<EntityShPtr>& m_entities;
      const std::vector<VFXShPtr>& m_vfx;

      std::vector<SortEntry> m_sortedBlocks;
      std::vector<SortEntry> m_sortedEntities;
      std::vector<SortEntry> m_sortedVFX;
  };

  using IteratorShPtr = std::shared_ptr<Iterator>;
}

# include "Iterators.hxx"

#endif    /* ITERATORS_HH */

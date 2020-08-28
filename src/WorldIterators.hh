#ifndef    WORLD_ITERATORS_HH
# define   WORLD_ITERATORS_HH

# include <core_utils/CoreObject.hh>
# include "WorldTypes.hh"

namespace new_frontiers {

  class WorldIterator: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new iterator on world's objects.
       * @param width - the width of the world in cells.
       * @param height - the height of the world in cells.
       * @param tiles - the list of tiles for this world.
       * @param entities - the list of entities in the world.
       * @param vfx - the list of vfx for the world.
       */
      WorldIterator(int width,
                    int height,
                    const std::vector<SolidTile>& tiles,
                    const std::vector<EntityTile>& entities,
                    const std::vector<VFXTile>& vfx);

      ~WorldIterator() = default;

      int
      w() const noexcept;

      int
      h() const noexcept;

      int
      solidTilesCount() const noexcept;

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
      const SolidTile&
      solidTile(int id) const noexcept;

      const EntityTile&
      entity(int id) const noexcept;

      const VFXTile&
      vfx(int id) const noexcept;

    private:

      int m_w;
      int m_h;

      const std::vector<SolidTile>& m_tiles;
      const std::vector<EntityTile>& m_entities;
      const std::vector<VFXTile>& m_vfx;
  };

}

# include "WorldIterators.hxx"

#endif    /* WORLD_ITERATORS_HH */

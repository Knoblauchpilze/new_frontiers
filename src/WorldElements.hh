#ifndef    WORLD_ELEMENTS_HH
# define   WORLD_ELEMENTS_HH

# include "WorldTypes.hh"
# include "TimeUtils.hh"
# include "RNG.hh"

namespace new_frontiers {

  template <typename TileType>
  class WorldElement {
    public:

      const Tile<TileType>&
      getDesc() const noexcept;

    protected:

      WorldElement(const Tile<TileType>& desc);

    private:

      Tile<TileType> m_tile;
  };

  class SolidElement: public WorldElement<Sprite> {
    public:

      SolidElement(const SolidTile& desc);
  };

  class Entity: public WorldElement<Mob> {
    public:

      Entity(const EntityTile& desc);
  };

  /**
   * @brief - Defines a portal in the game. A portal is the
   *          main source of enemies as it is able to spawn
   *          them in the world at specific intervals.
   *          Note that the interval between two consecutive
   *          mobs spawn is expressed in milliseconds.
   */
  class Spawner: public SolidElement {
    public:

      Spawner(const SolidTile& desc);

      bool
      depleted() const noexcept;

      bool
      canSpawn(const TimeStamp& now) const noexcept;

      Entity
      spawn(RNG& rng) noexcept;

    private:

      Duration m_interval;
      TimeStamp m_last;

      int m_toSpawn;
      int m_spawned;

      Mob m_mob;
      int m_mobID;

      float m_radius;
  };

}

# include "WorldElements.hxx"

#endif    /* WORLD_ELEMENTS_HH */

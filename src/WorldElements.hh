#ifndef    WORLD_ELEMENTS_HH
# define   WORLD_ELEMENTS_HH

# include "WorldTypes.hh"
# include "TimeUtils.hh"
# include "RNG.hh"
# include <memory>
# include <core_utils/CoreObject.hh>

namespace new_frontiers {

  template <typename TileType>
  class WorldElement: public utils::CoreObject {
    public:

      const Tile<TileType>&
      getDesc() const noexcept;

    protected:

      WorldElement(const Tile<TileType>& desc,
                   const std::string& name);

    protected:

      Tile<TileType> m_tile;
  };

  class VFX: public WorldElement<Effect> {
    public:

      VFX(const VFXTile& desc);

      virtual bool
      step(RNG& rng);

    private:

      int m_transitions;

      TimeStamp m_origin;
      Duration m_decay;
      Duration m_lastDecay;

      TimeStamp m_phase;

  };

  using VFXShPtr = std::shared_ptr<VFX>;

  class Entity: public WorldElement<Mob> {
    public:

      Entity(const EntityTile& desc,
             const Effect& vfx);

      virtual void
      step(std::vector<VFXShPtr>& created, RNG& rng);

    private:

      Effect m_vfx;

      Duration m_vfxDelta;
      TimeStamp m_last;
  };

  using EntityShPtr = std::shared_ptr<Entity>;

  class SolidElement: public WorldElement<Sprite> {
    public:

      virtual void
      step(std::vector<EntityShPtr>& created, RNG& rng) = 0;

    protected:

      SolidElement(const SolidTile& desc,
                   const std::string& name);
  };

  using SolidElementShPtr = std::shared_ptr<SolidElement>;

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

      void
      step(std::vector<EntityShPtr>& created, RNG& rng) override;

    private:

      bool
      depleted() const noexcept;

      bool
      canSpawn() const noexcept;

      EntityShPtr
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

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

# include "WorldElements.hxx"

#endif    /* WORLD_ELEMENTS_HH */

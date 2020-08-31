#ifndef    SPAWNER_HH
# define   SPAWNER_HH

# include <memory>
# include "SolidElement.hh"
# include "Entity.hh"

namespace new_frontiers {

  /**
   * @brief - Defines a portal in the game. A portal is the
   *          main source of enemies as it is able to spawn
   *          them in the world at specific intervals.
   *          Note that the interval between two consecutive
   *          mobs spawn is expressed in milliseconds.
   */
  class Spawner: public SolidElement {
    public:

      /**
       * @brief - Build a new portal with its associated visual
       *          display. A portal is a device allowed to spawn
       *          new mobs into the game using a custom strategy.
       * @param tile - the visual representation of the spawner
       *               along with its position.
       */
      Spawner(const SolidTile& tile);

      /**
       * @brief - Implementation of the interface method to step
       *          the spawner ahead in time. It will mainly be
       *          used to spawn new entities according to this
       *          portal's strategy.
       * @param info - the needed information about the world.
       * @return - `true` if an entity has been spawned.
       */
      bool
      step(StepInfo& info) override;

    private:

      /**
       * @brief - Convenience method to determine whether the
       *          portal still has some entities to spawn.
       * @return - `true` if no more entities can be spawned.
       */
      virtual bool
      depleted() const noexcept;

      /**
       * @brief - Return `true` in case the temporary condition
       *          are met for this portal to spawn a new entity.
       *          Note that this does not account for other
       *          rules that might be enforced for the portal to
       *          actually be able to spawn an entity.
       * @param moment - the current time which is used as a
       *                 base to determine whether an entity can
       *                 be spawned.
       * @return - `true` if an entity can be spawned.
       */
      virtual bool
      canSpawn(const TimeStamp& moment) const noexcept;

      /**
       * @brief - Create an entity conform to the specifications
       *          of this portal and return it as a pointer.
       * @param rng - used to introduce some sort of randomness
       *              in the generation of the entity.
       * @return - a pointer to the created entity.
       */
      EntityShPtr
      spawn(RNG& rng) noexcept;

    private:

      /** TODO: Comment. **/
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

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */

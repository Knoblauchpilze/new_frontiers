#ifndef    SPAWNER_HH
# define   SPAWNER_HH

# include <memory>
# include "SolidElement.hh"
# include "Entity.hh"

namespace new_frontiers {

  class Spawner: public SolidElement {
    public:

      /**
       * @brief - Build a new portal with its associated visual
       *          display. A portal is a device allowed to spawn
       *          new mobs into the game using a custom strategy.
       *          Also defines the type of mob spawned by this
       *          element, along with its variant.
       *          Note that this variant produces a portal with
       *          an infinite amount of mobs to spawn.
       * @param tile - the visual representation of the spawner
       *               along with its position.
       * @param mob - the type of mob spawned by this element.
       * @param id - the variant of mob spawned.
       */
      Spawner(const SolidTile& tile,
              const Mob& mob,
              int id = 0);

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
       * @param info - used to introduce some sort of randomness
       *               in the generation of the entity.
       * @return - a pointer to the created entity.
       */
      EntityShPtr
      spawn(StepInfo& info) noexcept;

    private:

      /**
       * @brief - The index of the mob which is spawned by this
       *          spawner.
       */
      Mob m_mob;

      /**
       * @brief - The variant of the mob spawned by this object.
       *          It should be consistent with the mob's type.
       */
      int m_mobID;

      /**
       * @brief - The number of mobs to spawn. This value is set
       *          to `-1` in case an infinite amount of mobs can
       *          be spawned.
       */
      int m_toSpawn;

      /**
       * @brief - The amount of mobs already spawned by this
       *          element. Increases indefinitely in case the
       *          spawner can always spawn mobs.
       */
      int m_spawned;

      /**
       * @brief - The interval to consider spawning a new mob.
       *          It is expressed in milliseconds and attempts
       *          will only occur when an interval expires.
       */
      Duration m_interval;

      /**
       * @brief - The timestamp at which a mob was generated
       *          last by this spawner.
       */
      TimeStamp m_last;

      /**
       * @brief - The radius around this spawner where a mob
       *          can be spawned. A value of `0` indicates
       *          that mobs should be spawn inside the portal.
       */
      float m_radius;

      /**
       * @brief - A threshold to prevent spawning mobs when a
       *          certain number are already existing close
       *          enough from the spawner.
       */
      int m_threshold;
  };

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */

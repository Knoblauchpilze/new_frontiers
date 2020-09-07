#ifndef    SPAWNER_HH
# define   SPAWNER_HH

# include <memory>
# include "Block.hh"
# include "../entities/Entity.hh"

namespace new_frontiers {

  class Spawner: public Block {
    public:

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

    protected:

      /**
       * @brief - Build a new portal with its associated visual
       *          display. A portal is a device allowed to spawn
       *          new mobs into the game using a custom strategy.
       *          Also defines the type of mob spawned by this
       *          element, along with its variant.
       *          The user should also provide a radius which is
       *          used to spawn entities on a circle around the
       *          portal. In case it is not needed the value of
       *          `0` indicates that entities should be spawned
       *          in the portal.
       * @param tile - the visual representation of the spawner
       *               along with its position.
       * @param radius - the radius of the circle into which the
       *                 spawner is allowed to create entities.
       * @param mob - the type of mob spawned by this element.
       * @param id - the variant of mob spawned.
       */
      Spawner(const BlockTile& tile,
              float radius,
              const tiles::Entity& mob,
              int id = 0);

    private:

      /**
       * @brief - Interface method to determine whether this
       *          spawner is able to create a new entity.
       *          Inheriting classes can implement this with
       *          logic of their own.
       * @param info - information about the current state of
       *               the world which can be used to define
       *               whether an entity can be spawned.
       * @return - `true` if an entity can be spawned.
       */
      virtual bool
      canSpawn(StepInfo& info) const noexcept = 0;

      /**
       * @brief - Create an entity conform to the specifications
       *          of this portal and return it as a pointer.
       * @param info - used to introduce some sort of randomness
       *               in the generation of the entity.
       * @return - a pointer to the created entity.
       */
      EntityShPtr
      spawn(StepInfo& info) noexcept;

      /**
       * @brief - Interface method guaranteed to be called when
       *          a successful spawn of an entity occured. It
       *          can be used by inheriting classes to update
       *          internal attributes in case an entity has been
       *          spawned.
       *          It is more reliable than assuming an entity
       *          will be spawned when `canSpawn` returns `true`.
       * @param info - the information about the spawn environment.
       * @param ent - the entity that has been spawned.
       */
      virtual void
      preSpawn(StepInfo& info, EntityShPtr ent) = 0;

    private:

      /**
       * @brief - The index of the mob which is spawned by this
       *          spawner.
       */
      tiles::Entity m_mob;

      /**
       * @brief - The variant of the mob spawned by this object.
       *          It should be consistent with the mob's type.
       */
      int m_mobID;

      /**
       * @brief - The radius around this spawner where a mob
       *          can be spawned. A value of `0` indicates
       *          that mobs should be spawn inside the portal.
       *          This value is clamped to `0`.
       */
      float m_radius;
  };

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */

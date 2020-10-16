#ifndef    SPAWNER_HH
# define   SPAWNER_HH

# include <memory>
# include "Block.hh"
# include "../entities/Entity.hh"

namespace new_frontiers {

  namespace mob {

    /**
     * @brief - The general type of mobs that can be spawned.
     */
    enum class Type {
      Worker,
      Warrior
    };

    /**
     * @brief - Used to convert a string into a valid
     *          mob type value. In case the string does
     *          not correspond to any known mob type a
     *          `Worker` value is returned and the error
     *          flag is set to `true`.
     * @param str - the string to convert.
     * @param error - `true` if the string could not be
     *                decoded.
     * @return - the corresponding mob type.
     */
    Type
    strToType(const std::string& str, bool& error) noexcept;

  }

  class Spawner: public Block {
    public:

      /**
       * @brief - Implementation of the interface method to step
       *          the spawner ahead in time. It will mainly be
       *          used to spawn new entities according to this
       *          portal's strategy.
       * @param info - the needed information about the world.
       */
      void
      step(StepInfo& info) override;

    protected:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a spawner. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct SProps: Props {
        tiles::Entity mob;
        int mVariant;

        mob::Type agent;

        float spawnRadius;
      };

      /**
       * @brief - Build a new portal with its associated visual
       *          display. A portal is a device allowed to spawn
       *          new mobs into the game using a custom strategy.
       *          The `props` argument defines the properties to
       *          use to describe the spawning strategy and some
       *          additional information to generate the spawner.
       * @param props - a struct describing the spawner's behavior.
       */
      Spawner(const SProps& props);

      /**
       * @brief - Interface method to allow inheriting classes
       *          to perform changes in their internal values
       *          upon each execution of the `step` method.
       * @param info - the information about the world to use
       *               when performing the update.
       */
      virtual void
      update(StepInfo& info) = 0;

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
       * @brief - The type of IA to plug to the bodies of the
       *          entities spawned by this object.
       */
      mob::Type m_type;

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
      float m_spawnRadius;
  };

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */

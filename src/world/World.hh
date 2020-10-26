#ifndef    WORLD_HH
# define   WORLD_HH

# include <memory>
# include <fstream>
# include <core_utils/CoreObject.hh>
# include "RNG.hh"
# include "Tiles.hh"
# include "colonies/Colony.hh"
# include "Element.hh"
# include "Locator.hh"
# include "Controls.hh"
# include "Influence.hh"
# include "blocks/Deposit.hh"
# include "effects/Pheromon.hh"

namespace new_frontiers {

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new world with the specified dimensions.
       * @param seed - the seed to use for this world.
       * @param width - the width of the world in cells.
       * @param height - the height of the window in cells.
       */
      World(int seed, int width = 8, int height = 6);

      /**
       * @brief - Build a new world from the file specified in input.
       *          The file should describe a valid level.
       * @param seed - the seed to use for this world.
       * @param file - the name of the file from which the world's
       *               data should be loaded.
       */
      World(int seed,
            const std::string& file);

      /**
       * @brief - Desctruction of the object.
       */
      ~World();

      unsigned
      w() const noexcept;

      unsigned
      h() const noexcept;

      /**
       * @brief - Used to return a locator object to allow easy
       *          access to elements of this world.
       * @return - a locator allowing to filter and efficiently
       *           access objects of this world.
       */
      LocatorShPtr
      locator() const noexcept;

      /**
       * @brief - Used to move one step ahead in time in this
       *          world, given that `tDelta` represents the
       *          duration of the last frame in seconds. The
       *          method is also given the controls as input
       *          so that it can be made available to entities.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       * @param controls - the current state of the controls.
       */
      void
      step(float tDelta,
           const controls::State& controls);

      /**
       * @brief - Used to indicate that the world should be
       *          paused. Time based entities and actions
       *          should take actions to correctly resume at
       *          a later time.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       * @param controls - the current state of the controls.
       */
      void
      pause(float tDelta,
            const controls::State& controls);

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       * @param controls - the current state of the controls.
       */
      void
      resume(float tDelta,
             const controls::State& controls);

      /**
       * @brief - Used to assign a new value for the properties
       *          of a block to create as an action. This will
       *          be used in the following calls when a new game
       *          elements need to be created.
       * @param props - the properties of the block to create.
       */
      void
      setBlockProps(BlockPropsShPtr props);

      /**
       * @brief - Very similar to `setBlockProps` but defines the
       *          properties to use when spawning an entity.
       * @param props - the properties to use when spawning a new
       *                entity.
       */
      void
      setEntityProps(EntityPropsShPtr props);

      /**
       * @brief - Very similar to `setBlockProps` but defines the
       *          properties to use when spawning a vfx.
       * @param props - the properties to use when spawning a new
       *                vfx.
       */
      void
      setVFXProps(VFXPropsShPtr props);

      /**
       * @brief - Used to perform the next action at the position
       *          specified by the input coordinates.
       * @param x - the abscissa of the position at which the action
       *            should be taking place.
       * @param y - the ordinate of the position at which the action
       *            will be taking place.
       */
      void
      performAction(float x, float y);

    private:

      /**
       * @brief - Perform the initialization of the actions
       *          defined in the world.
       */
      void
      initializeActions();

      /**
       * @brief - Generate the base layout for this world.
       */
      void
      generate();

      /**
       * @brief - Used to perform the generation of elements
       *          of the world such as solid tiles or all the
       *          entities existing at the beginning of the
       *          game.
       */
      void
      generateElements();

      /**
       * @brief - Used to process the input list of influences
       *          which usually mean deleting elements marked
       *          for deletion and registering new ones.
       *          The influences are fetched from the internal
       *          vector registering them. This vector is then
       *          reset so as to indicate that all influences
       *          have been handled.
       */
      void
      processInfluences();

      /**
       * @brief - Attempt to load a world from the file as
       *          specified in input.
       * @param file - the name of the file from which the
       *               world should be loaded.
       */
      void
      loadFromFile(const std::string& file);

      /**
       * @brief - Used to parse the dimensions of the world
       *          from the file laoded in the stream.
       *         Throws an error in case an error occurs.
       * @param in - the stream describing the level file.
       */
      void
      loadDimensions(std::ifstream& in);

      void
      loadColonies(std::ifstream& in);

      void
      loadPortals(std::ifstream& in);

      void
      loadBlocks(std::ifstream& in);

      void
      loadEntities(std::ifstream& in);

    private:

      /**
       * @brief - Convenience define determining which kind of
       *          action is currently `selected`. This means
       *          that any call to `performAction` will use the
       *          registered `action` for this type.
       */
      enum class ActionType {
        None,
        Block,
        Entity,
        VFX
      };

      /**
       * @brief - Convenience structure allowing to represent the
       *          actions currently registered in this world. It
       *          allows to have some sort of delay processes that
       *          can be first setup and then activated as many
       *          times as needed.
       *          Typical examples include creating game elements
       *          from the UI.
       */
      struct Actions {
        BlockPropsShPtr block;
        EntityPropsShPtr ent;
        VFXPropsShPtr vfx;

        ActionType type;
      };

      /**
       * @brief - Width of the world in cells.
       */
      int m_w;

      /**
       * @brief - Height of the world in cells.
       */
      int m_h;

      /**
       * @brief - The random number engine for this world: allows to
       *          make tthe simulation deterministic by gathering all
       *          randomness in a single place.
       */
      RNG m_rng;

      /**
       * @brief - The list of colonies registered in the world.
       */
      std::vector<ColonyShPtr> m_colonies;

      /**
       * @brief - The list of blocks for this world.
       */
      std::vector<BlockShPtr> m_blocks;

      /**
       * @bvrief - The list of monsters and entities that have spawned
       *           in the world.
       */
      std::vector<EntityShPtr> m_entities;

      /**
       * @brief - The list of visual effects currently existing in the
       *          game. Effects that have finished are erased from the
       *          list.
       */
      std::vector<VFXShPtr> m_vfx;

      /**
       * @brief - An object to hold all the tiles and entities that
       *          have been registered so far in the world, stored in
       *          a way that maximize efficiency for location-based
       *          queries.
       *          Very useful for entities behavior that are usually
       *          based on locating specific tiles in the game.
       */
      LocatorShPtr m_loc;

      /**
       * @brief - The list of actions that are registered in the
       *          world. Such actions are mainly used to create
       *          new game elements.
       *          Initial values are provided to create default
       *          elements.
       */
      Actions m_actions;

      /**
       * @brief - List of influences registered for this world. The
       *          influence concept is the main way any game element
       *          has to change its surroundings. All influences are
       *          first gathered and then all are applied at the same
       *          time. This allow a phase of resolution where we
       *          will determine which influence can be executed and
       *          possibly find some compromise for some cases like
       *          if several entities are competing to fetch the last
       *          resources from a deposit, etc.
       */
      std::vector<InfluenceShPtr> m_influences;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

# include "World.hxx"

#endif    /* WORLD_HH */

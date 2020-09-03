#ifndef    WORLD_HH
# define   WORLD_HH

# include <memory>
# include <fstream>
# include <core_utils/CoreObject.hh>
# include "RNG.hh"
# include "WorldTypes.hh"
# include "WorldIterators.hh"
# include "WorldElement.hh"
# include "WorldElementLocator.hh"

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
       * @brief - Used to return an iterator on this world.
       * @return - an iterator on the objects of this world.
       */
      WorldIteratorShPtr
      iterator() const noexcept;

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
           const std::vector<bool>& controls);

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
            const std::vector<bool>& controls);

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
             const std::vector<bool>& controls);

    private:

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
      loadEntrances(std::ifstream& in);

      void
      loadExits(std::ifstream& in);

      void
      loadPortals(std::ifstream& in);

      void
      loadWalls(std::ifstream& in);

      void
      loadEntities(std::ifstream& in);

    private:

      /**
       * @brief - Dimensions of the world in cells.
       */
      int m_w;
      int m_h;

      /**
       * @brief - The random number engine for this world: allows to
       *          make tthe simulation deterministic by gathering all
       *          randomness in a single place.
       */
      RNG m_rng;

      /**
       * @brief - The list of tiles for this world.
       */
      std::vector<SolidElementShPtr> m_tiles;

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
       * @brief - The iterator on this world. Will be used as a return
       *          value in the `iterator` method so that everyone gets
       *          the same one.
       */
      WorldIteratorShPtr m_it;

      /**
       * @brief - An object to hold all the tiles and entities that
       *          have been registered so far in the world, stored in
       *          a way that maximize efficiency for location-based
       *          queries.
       *          Very useful for entities behavior that are usually
       *          based on locating specific tiles in the game.
       */
      WorldElementLocatorShPtr m_loc;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

# include "World.hxx"

#endif    /* WORLD_HH */

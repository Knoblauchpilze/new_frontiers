#ifndef    WORLD_HH
# define   WORLD_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "RNG.hh"
# include "WorldTypes.hh"
# include "WorldIterators.hh"

namespace new_frontiers {

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new world with the specified dimensions.
       * @param seed - the seed to use for this world.
       * @param width - the width of the world in cells.
       * @param height - the height of the window in cells.
       */
      World(int seed,
            int width = 640,
            int height = 480);

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
       *          world, given that `tDelta` TODO: unit has
       *          passed since the last frame.
       * @param tDelta - TODO: comment.
       */
      void
      step(float tDelta);

    private:

      /**
       * @brief - Generate the base layout for this world.
       */
      void
      generate();

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
      std::vector<SolidTile> m_tiles;

      /**
       * @bvrief - The list of monsters and entities that have spawned
       *           in the world.
       */
      std::vector<EntityTile> m_entities;

      /**
       * @brief - The list of visual effects currently existing in the
       *          game. Effects that have finished are erased from the
       *          list.
       */
      std::vector<VFXTile> m_vfx;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

# include "World.hxx"

#endif    /* WORLD_HH */

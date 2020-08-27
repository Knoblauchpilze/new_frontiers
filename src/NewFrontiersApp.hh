#ifndef    NEW_FRONTIERS_APP_HH
# define   NEW_FRONTIERS_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"
# include <random>

namespace new_frontiers {

  class NewFrontiersApp: public utils::CoreObject, public olc::PixelGameEngine {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       * @param appName - The name of the application to create.
       */
      NewFrontiersApp(unsigned width = 640u,
                      unsigned height = 480u,
                      const std::string& appName = "new_frontiers");

      /**
       * @brief - Desctruction of the object.
       */
      ~NewFrontiersApp();

      /**
       * @brief - Implementation of the interface method called
       *          during the creation of the application.
       * @return - `true` if the initialization succeeded.
       */
      bool
      OnUserCreate() override;

      /**
       * @brief - Override of the main update function called at
       *          each frame.
       * @param fElapsedTime - the duration elapsed since last
       *                       frame.
       * @return - `true` if the update succeeded.
       */
      bool
      OnUserUpdate(float fElapsedTime) override;

    private:

      /**
       * @brief - Returns the size of the sprite in the input image
       *          file. This size includes the potential whitespace
       *          included in the resource pack to accomodate for
       *          the ground textures.
       * @return - the sprite size in pixels.
       */
      static
      olc::vi2d
      spriteSize() noexcept;

      /**
       * @brief - Used to convert from sprite coordinates to the
       *          corresponding pixels coordinates. This method
       *          should mostly be used to locate a sprite in a
       *          resource pack.
       * @param x - the x coordinate of the sprite in the pack.
       * @param y - the y coordinate of the sprite in the pack.
       * @return - a vector representing the pixels coordinates
       *           for the input sprite coords.
       */
      static
      olc::vi2d
      spriteCoordsToPixels(int x, int y) noexcept;

      /**
       * @brief - Represent the size of a tile in the world as
       *          displayed on the screen. This size is at most
       *          equal to the sprite size and is usually a bit
       *          smaller because the sprite size include the
       *          characters which makes it higher.
       * @return - the tile size in pixels.
       */
      static
      olc::vi2d
      tileSize() noexcept;

      /**
       * @brief - Used to convert from tile coordinates to pixel
       *          coordinates. This method can be used when some
       *          tile is to be displayed on the screen. We make
       *          use of a global world origin that represents
       *          an offset applied to the world to make it more
       *          appealing and visible.
       * @param tiles - the position to convert in tiles coords.
       * @param full - `ŧrue` if the tile associated to the pos
       *               is a full `tile` (i.e. a full sprite) or
       *               `false` in case it has no height element.
       * @return - the coordinates in pixels of the tile defined
       *           by the input coords.
       */
      static
      olc::vi2d
      tileCoordsToPixels(const olc::vi2d& tiles, bool full) noexcept;

      /**
       * @brief - Convert from pixels coordinates to tile coords.
       *          Some extra logic is added in order to account
       *          for the tiles that do not align with the grid
       *          so that we always get an accurate position for
       *          the tile.
       * @param pixels - the pixels coordinates to convert into
       *                 tile coords.
       * @return - the corresponding tile coordinates.
       */
      static
      olc::vi2d
      pixelCoordsToTiles(const olc::vi2d& pixels) noexcept;

      /**
       * @brief - Performs the initialization of the engine to make
       *          it suits our needs.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       */
      void
      initialize(unsigned width,
                 unsigned height);

      void
      createTileAliases();

      void
      generatePortals();

      void
      spawnMonster();

    private:

      static constexpr int32_t WORLD_ORIGIN_X = 5;
      static constexpr int32_t WORLD_ORIGIN_Y = 1;

      static constexpr int32_t WORLD_SIZE_W = 10;
      static constexpr int32_t WORLD_SIZE_H = 11;

      static constexpr int32_t PORTALS_COUNT = 2;
      static constexpr int32_t ENEMIES_COUNT = 4;

      enum Sprite {
        Portal,
        Monster,
        Ground,
        Door,
        Cursor
      };

      struct Tile {
        olc::vi2d coords;
        bool full;
      };

      static constexpr unsigned TileCount = 5u;

      struct RNGUtils {
        std::random_device device;
        std::mt19937 rng;

        std::uniform_int_distribution<int> wRNG;
        std::uniform_int_distribution<int> hRNG;

        olc::vi2d
        coords();

        void
        reset(unsigned w,
              unsigned h);
      };

      olc::Sprite* m_sprite;
      std::vector<Tile> m_aliases;

      std::vector<olc::vi2d> m_portals;
      std::vector<olc::vi2d> m_enemies;

      RNGUtils m_random;

      bool m_first;
  };

}

# include "NewFrontiersApp.hxx"

#endif    /* NEW_FRONTIERS_APP_HH */

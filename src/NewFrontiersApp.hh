#ifndef    NEW_FRONTIERS_APP_HH
# define   NEW_FRONTIERS_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"
# include <random>
# include "World.hh"

namespace new_frontiers {

  class NewFrontiersApp: public utils::CoreObject, public olc::PixelGameEngine {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       * @param pixelRatio - defines the ratio between an viewport
       *                     pixel and a screen pixel.
       * @param appName - The name of the application to create.
       */
      NewFrontiersApp(int width = 640,
                      int height = 480,
                      int pixelRatio = 1,
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
      olc::vi2d
      spriteCoordsToPixels(int x, int y) const noexcept;

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
       * @param x - the cell coordinate along the `x` axis.
       * @param y - the cell coordinate along the `y` axis.
       * @return - the coordinates in pixels of the tile defined
       *           by the input coords.
       */
      olc::vi2d
      tileCoordsToPixels(int x, int y) const noexcept;

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
      olc::vi2d
      pixelCoordsToTiles(const olc::vi2d& pixels) const noexcept;

      /**
       * @brief - Performs the initialization of the engine to make
       *          it suits our needs.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       * @param pixelRatio - the ratio between a viewport pixel and
       *                     a screen pixel.
       */
      void
      initialize(int width, int height, int pixelRatio);

      /**
       * @brief - Perform the creation of the tile aliases, allowing
       *          to easily associate a sprite with it's visual item
       *          and speed up the rendering.
       *          The layout of the alias is packed so that all tiles
       *          are contiguously stored in memory and one can use
       *          the below methods to convert from a specific type
       *          to the index in the alias.
       */
      void
      createTileAliases();

      /**
       * @brief - Compute the index of the input sprite in the atlas
       *          array.
       * @param sprite - the sprite for which the alias index should
       *                 be computed.
       * @return - the index of the sprite in the alias array.
       */
      static
      int
      aliasOfSprite(const Sprite& sprite);

      /**
       * @brief - Compute the index of the input entity in the atlas
       *          array.
       * @param ent - the entity for which the alias index should be
       *              computed.
       * @return - the index of the entity in the alias array.
       */
      static
      int
      aliasOfEntity(const Mob& ent);

      /**
       * @brief - Compute the index of the input effect in the atlas
       *          array.
       * @param vfx - the effect for which the alias index should be
       *              computed.
       * @return - the index of the effect in the alias array.
       */
      static
      int
      aliasOfEffect(const Effect& vfx);

      /**
       * @brief - Used to draw the tile referenced by its alias index
       *          and at the coordinate `cell`.
       * @param x - the coordinate along the `x` axis for this sprite.
       * @param y - the coordinate along the `y` axis for this sprite.
       * @param alias - the index of the tile in the tile atlas.
       */
      void
      drawSprite(int x, int y, int alias);

    private:

      /**
       * @brief - Holds the main sprite defining the visual aspect
       *          of in-game elements. It is usually represented as
       *          a large texture containing separate elements for
       *          each tile type.
       */
      olc::Sprite* m_sprite;

      /**
       * @brief - Defines an atlas where each tile type is stored
       *          for easy access inside the general sprite. It
       *          allows to easily associate a tile type with a
       *          visual element.
       */
      std::vector<olc::vi2d> m_aliases;

      /**
       * @brief - Defines the coordinates of the top part of the
       *          world in cells' coordinates. Allows to offset
       *          the representation of the world on-screen and
       *          thus allow panning/zooming etc.
       *          The `m_wox` stands for `World origin along the
       *          x axis` and the `m_woy` stands for a similar
       *          name but for the `y` axis.
       */
      int m_wox;
      int m_woy;

      /**
       * @brief - The world managed by this application.
       */
      WorldShPtr m_world;
  };

}

# include "NewFrontiersApp.hxx"

#endif    /* NEW_FRONTIERS_APP_HH */

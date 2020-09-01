#ifndef    NEW_FRONTIERS_APP_HH
# define   NEW_FRONTIERS_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"
# include <random>
# include "World.hh"
# include "CoordinateFrames.hh"

namespace new_frontiers {

  /**
   * @brief - Convenience structure allowing to represent the
   *          sprites inside a file. In addition to the name
   *          of the file the layout of the sprites is also
   *          specified so as to be able to identify tiles in
   *          a correct way.
   */
  struct SpriteFile {
    std::string file;
    olc::vi2d layout;
  };

  /**
   * @brief - Define the common information for the theme to be
   *          used in this application. It defines both names of
   *          the sprite files along with information about the
   *          layout in each one.
   *          This information is then interpreted by the app to
   *          correctly render the tiles.
   */
  struct Theme {
    // Sprite files.
    SpriteFile solidTiles;
    SpriteFile portals;
    SpriteFile entities;
    SpriteFile vfx;
    SpriteFile cursors;

    olc::vi2d size;
  };

  class NewFrontiersApp: public utils::CoreObject, public olc::PixelGameEngine {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       * @param theme - the description of the theme to use
       *                for sprites in this application.
       * @param pixelRatio - defines the ratio between an viewport
       *                     pixel and a screen pixel.
       * @param appName - The name of the application to create.
       */
      NewFrontiersApp(int width,
                      int height,
                      const Theme& theme,
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
       * @brief - Convenience define indicating a fully opaque alpha.
       */
      static constexpr int ALPHA_OPAQUE = 255;

      /**
       * @brief - Not fully opaque nor fully transparent.
       */
      static constexpr int ALPHA_SEMI_OPAQUE = 128;

      /**
       * @brief - Convenience define indicating a fully transparent alpha.
       */
      static constexpr int ALPHA_TRANSPARENT = 0;

      /**
       * @brief - Performs the initialization of the engine to make
       *          it suits our needs.
       * @param width - the width of the window in pixels.
       * @param height - the height of the window in pixels.
       * @param theme - the theme to use for this application.
       * @param pixelRatio - the ratio between a viewport pixel and
       *                     a screen pixel.
       */
      void
      initialize(int width, int height, const Theme& theme, int pixelRatio);

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
       * @brief - Used to convert from sprite coordinates to the
       *          corresponding pixels coordinates. This method
       *          should mostly be used to locate a sprite in a
       *          resource pack. The provided layout allows to
       *          know in advance how sprites are laid out in
       *          the pack and thus find the correct location
       *          based on the coordinates of the sprite and
       *          its identifier.
       *          In order to find the correct sprite, both some
       *          coordinates and a variation id should be set
       *          to fix a single element in the sprites.
       * @param coord - the coordinates of the sprite to convert
       *                to pixels in the resource pack.
       * @param layout - defines the layout of the sprites in
       *                 the resource pack.
       * @param id - the index of the variation of the sprite
       *             to use: default is `0`.
       * @return - a vector representing the pixels coordinates
       *           for the input sprite coords.
       */
      olc::vi2d
      spriteCoordsToPixels(const olc::vi2d& coord, const olc::vi2d& layout, int id = 0) const noexcept;

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
       * @param id - the index of the variation of this sprite.
       * @param alpha - a value of `255` to indicate that the sprite
       *                should be completely opaque and `0` for fully
       *                transparent.
       */
      void
      drawSprite(float x, float y, int alias, int id, int alpha = ALPHA_OPAQUE);

      /**
       * @brief - Used to perform the necessary update based on
       *          the controls that the user might have used in
       *          the game.
       * @return - `true` if no interruption was detected (and
       *           thus the execution should continue). Meant
       *           as a way to provide the return value for the
       *           `OnUserUpdate` method.
       */
      bool
      handleInputs();

      /**
       * @brief - Perform the rendering of the world on the
       *          display device.
       */
      void
      draw();

      /**
       * @brief - Used to perform debug rendering in addition to
       *          the standard display routines.
       */
      void
      drawDebug();

    private:

      /**
       * @brief - Convenience enumeration to refer to the position of
       *          each sprite type in the `m_sprites` array.
       */
      enum SpriteLocation {
        SolidID,
        PortalID,
        EntityID,
        VFXID,
        CursorID,

        SpriteTypesCount
      };

      /**
       * @brief - Convenience define allowing to refer to an alias in
       *          a sprite file: it contains both the sprite's type
       *          along with its actual position in the resource pack.
       */
      struct SpriteAlias {
        SpriteLocation type;
        olc::vi2d alias;
      };

      /**
       * @brief - Convenience define allowing to represent a resource
       *          pack within the app. It mostly consists in a single
       *          `decal` (understand sprite) resource and the layout
       *          associated to it.
       *          In order to allow deferred loading we provide the
       *          name of the file defining this pack.
       */
      struct SpritesPack {
        std::string file;
        olc::Decal* res;
        olc::vi2d layout;
      };

      /**
       * @brief - Define the size of a sprtie in pixels as loaded
       *          from the resource packs used by this application.
       *          We allow only a single size in order to maintain
       *          some sort of aspect consistency across the app.
       */
      olc::vi2d m_ss;

      /**
       * @brief - Holds the main sprites defining the visual aspect
       *          of in-game elements. It is usually represented as
       *          some large textures containing separate elements
       *          for each tile type.
       *          Note that the `olc` type is a decal but it does
       *          not change the fact that we're displaying sprites.
       *          The `decal` is just more powerful in terms of
       *          scaling and rotation which makes it more suited
       *          for the viewport handling for example.
       */
      std::vector<SpritesPack> m_sprites;

      /**
       * @brief - Defines an atlas where each tile type is stored
       *          for easy access inside the general sprite. It
       *          allows to easily associate a tile type with a
       *          visual element.
       */
      std::vector<SpriteAlias> m_aliases;

      /**
       * @brief - The world managed by this application.
       */
      WorldShPtr m_world;

      /**
       * @brief - The world iterator retrieved on the world: used
       *          to traverse the elements defined in the world to
       *          display them.
       */
      WorldIteratorShPtr m_wit;

      /**
       * @brief - Holds an object allowing to convert between the
       *          various coordinate frames handled by the app. It
       *          handles conversion between cells coordinate and
       *          screen coordinates and conversely.
       */
      CoordinateFrames m_cf;

      /**
       * @brief - A map to keep track of the state of the controls
       *          to be transmitted to the world's entities for
       *          the simulation.
       */
      std::vector<bool> m_controls;

      bool m_first;
  };

}

# include "NewFrontiersApp.hxx"

#endif    /* NEW_FRONTIERS_APP_HH */

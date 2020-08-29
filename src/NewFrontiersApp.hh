#ifndef    NEW_FRONTIERS_APP_HH
# define   NEW_FRONTIERS_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"
# include <random>
# include "World.hh"
# include "CoordinateFrames.hh"

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

    private:

      /**
       * @brief - Holds the main sprite defining the visual aspect
       *          of in-game elements. It is usually represented as
       *          a large texture containing separate elements for
       *          each tile type.
       *          Note that the `olc` type is a decal but it does
       *          not change the fact that we're displaying sprites.
       *          The `decal` is just more powerful in terms of
       *          scaling and rotation which makes it more suited
       *          for the viewport handling for example.
       */
      olc::Decal* m_sprite;

      /**
       * @brief - Defines the size of the sprite in the original
       *          source file. Allows mainly to draw the correct
       *          piece of texture when representing an element.
       */
      olc::vi2d m_ss;

      /**
       * @brief - Defines an atlas where each tile type is stored
       *          for easy access inside the general sprite. It
       *          allows to easily associate a tile type with a
       *          visual element.
       */
      std::vector<olc::vi2d> m_aliases;

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
  };

}

# include "NewFrontiersApp.hxx"

#endif    /* NEW_FRONTIERS_APP_HH */

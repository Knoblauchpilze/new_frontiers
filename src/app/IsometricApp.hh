#ifndef    ISOMETRIC_APP_HH
# define   ISOMETRIC_APP_HH

# include "PGEApp.hh"
# include <unordered_map>
# include "ColorGenerator.hh"

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

  class IsometricApp: public PGEApp {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param dims - the dimensions of the application's main
       *               canvas in pixels. The ratio will be set
       *               to `1x1`.
       * @param theme - the description of the theme to use
       *                for sprites in this application.
       * @param name - the name of the application.
       */
      IsometricApp(const olc::vi2d& dims,
                   const Theme& theme,
                   const std::string& name = "new_frontiers");

      /**
       * @brief - Desctruction of the object.
       */
      ~IsometricApp() = default;

    protected:

      /**
       * @brief - Redefinition of the interface method to perform the
       *          loading of the sprites needed for this application.
       */
      void
      loadResources() override;

      /**
       * @brief - Redefinition of the interface method to perform a
       *          cleaning of the sprites loaded to display the app.
       */
      void
      cleanResources() override;

      /**
       * @brief - Definition of the interface method to perform the
       *          drawing of the game's logic.
       * @param res - the resources that can be drawn.
       */
      void
      draw(const RenderDesc& res) override;

      /**
       * @brief - Similar to the `draw` method but performs the
       *          drawing of the app's interface.
       * @param res - the resources that can be drawn.
       */
      void
      drawUI(const RenderDesc& res) override;

      /**
       * @brief - Draws the debug layer for this app through the
       *          definition of the corresponding interface method.
       * @param res - the resources that can be drawn.
       */
      void
      drawDebug(const RenderDesc& res) override;

    private:

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
       * @brief - Compute the index of the input block in the atlas
       *          array.
       * @param block - the block for which the alias index should
       *                 be computed.
       * @return - the index of the block in the alias array.
       */
      static
      int
      aliasOfBlock(const tiles::Block& block);

      /**
       * @brief - Compute the index of the input entity in the atlas
       *          array.
       * @param ent - the entity for which the alias index should be
       *              computed.
       * @return - the index of the entity in the alias array.
       */
      static
      int
      aliasOfEntity(const tiles::Entity& ent);

      /**
       * @brief - Compute the index of the input effect in the atlas
       *          array.
       * @param vfx - the effect for which the alias index should be
       *              computed.
       * @return - the index of the effect in the alias array.
       */
      static
      int
      aliasOfEffect(const tiles::Effect& vfx);

      /**
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param tile - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawSprite(const SpriteDesc& tile, const CoordinateFrame& cf);

      /**
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param tile - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       * @param id - the index of the variation of this sprite.
       * @param tint - a tint to apply to the sprite (default
       *               is white so no change is applied).
       */
      void
      drawSprite(const SpriteDesc& tile,
                 const CoordinateFrame& cf,
                 int id,
                 const olc::Pixel& tint = olc::WHITE);

      /**
       * @brief - Used to fetch a color for the identifier of
       *          the colony provided in input.
       *          If this method fails a `White` color will be
       *          returned but this state won't be persisted
       *          giving a chance to restore the situation in
       *          the future.
       * @param colony - the identifier of the colony for which
       *                 a color should be generated as a string.
       * @return - the color for this colony.
       */
      olc::Pixel
      getColorFor(const std::string& colony) noexcept;

    private:

      /**
       * @brief - Convenience define to refer to the map of
       *          colors representing colonies.
       */
      using ColorMap = std::unordered_map<std::string, olc::Pixel>;

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
       * @brief - Used to generate new colors for colonies when
       *          needed.
       */
      ColorGenerator m_cGenerator;

      /**
       * @brief - Used to register each colony and assign a color
       *          to it so that it can be easily distinguished
       *          from other elements.
       */
      ColorMap m_coloniesColors;
  };

}

# include "IsometricApp.hxx"

#endif    /* ISOMETRIC_APP_HH */

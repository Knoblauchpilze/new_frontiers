#ifndef    ISOMETRIC_APP_HH
# define   ISOMETRIC_APP_HH

# include <random>
# include "PGEApp.hh"

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
       * @param desc - the base description for the canvas to
       *               be used by this app.
       * @param theme - the description of the theme to use
       *                for sprites in this application.
       */
      IsometricApp(const AppDesc& desc, const Theme& theme);

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
       * @brief - Used to draw the tile referenced by its alias index
       *          and at the coordinate `cell`.
       * @param x - the coordinate along the `x` axis for this sprite.
       * @param y - the coordinate along the `y` axis for this sprite.
       * @param alias - the index of the tile in the tile atlas.
       * @param id - the index of the variation of this sprite.
       * @param location - defines which part of the cell is described
       *                   by the input `(x, y)` coordinates. It allows
       *                   to precisely define where the sprite should
       *                   be displayed in the cell.
       * @param alpha - a value of `255` to indicate that the sprite
       *                should be completely opaque and `0` for fully
       *                transparent.
       */
      void
      drawSprite(float x,
                 float y,
                 int alias,
                 int id,
                 const Cell& location = Cell::TopLeft,
                 int alpha = ALPHA_OPAQUE);

      /**
       * @brief - Used to draw a minimalistic health bar for an entity
       *          or block at position (x, y) in cells coordinates. It
       *          is displayed with the specified alpha transparency.
       * @param x - the coordinate along the `x` axis for the element
       *            this healthbar is attached to.
       * @param y - the coordinate along the `y` axis for the element
       *            this healthbar is attached to.
       * @param ratio - a value in the range `[0; 1]` representing the
       *                health of the element.
       * @param location - the base location of the health bar compared
       *                   to the tile.
       * @param alpha - additional alpha modifier to blend the display.
       */
      void
      drawHealthBar(float x,
                    float y,
                    float ratio,
                    const Cell& location = Cell::TopLeft,
                    int alpha = ALPHA_SEMI_OPAQUE);

      /**
       * @brief - Used to compute a valid color based on the input ratio.
       *          The color is taken from a range going from pure green
       *          to yellow, then orange and finally red for decreasing
       *          values of the ratio in the range `[0; 1]`.
       * @param ratio - a value in the range `[0; 1]` (clamped if it is
       *                not the case).
       * @param alpha - additional alpha channel information to generate
       *                the color.
       * @return - a color representing the ratio.
       */
      olc::Pixel
      ratioGradient(float ratio, int alpha = ALPHA_SEMI_OPAQUE) const noexcept;


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
  };

}

# include "IsometricApp.hxx"

#endif    /* ISOMETRIC_APP_HH */

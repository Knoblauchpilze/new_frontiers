#ifndef    TOPVIEW_APP_HH
# define   TOPVIEW_APP_HH

# include "PGEApp.hh"

namespace new_frontiers {

  class TopViewApp: public PGEApp {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param dims - the dimensions of the application's main
       *               canvas in pixels. The ratio will be set
       *               to `1x1`.
       * @param name - the name of the application.
       */
      TopViewApp(const olc::vi2d& dims,
                 const std::string& name = "new_frontiers");

      /**
       * @brief - Desctruction of the object.
       */
      ~TopViewApp() = default;

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
       * @brief - Used to draw the tile referenced by its alias index
       *          and at the coordinate `cell`.
       * @param pos - the position at which the tile should be drawn.
       * @param tileSize - the size of the tile in pixels.
       * @param type - the type of the tile to draw.
       * @param alpha - a value of `255` to indicate that the sprite
       *                should be completely opaque and `0` for fully
       *                transparent.
       */
      void
      drawSprite(const olc::vf2d& pos,
                 const olc::vf2d& tileSize,
                 int type,
                 int alpha = ALPHA_OPAQUE);

      /**
       * @brief - Used to draw a minimalistic health bar for an entity
       *          or block at position (x, y) in cells coordinates. It
       *          is displayed with the specified alpha transparency.
       * @param pos - the position at which the tile should be drawn.
       * @param tileSize - the size of the tile in pixels.
       * @param ratio - a value in the range `[0; 1]` representing the
       *                health of the element.
       * @param alpha - additional alpha modifier to blend the display.
       */
      void
      drawHealthBar(const olc::vf2d& pos,
                    const olc::vf2d& tileSize,
                    float ratio,
                    int alpha = ALPHA_SEMI_OPAQUE);


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
       * @brief - Holds an array with colors defined for each in
       *          game element. It is a very minimalistic visual
       *          representation for now but it is sufficient.
       */
      std::vector<olc::Pixel> m_sprites;
  };

}

# include "TopViewApp.hxx"

#endif    /* TOPVIEW_APP_HH */

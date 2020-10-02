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
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param tile - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawSprite(const SpriteDesc& tile, const CoordinateFrame& cf);

    private:

      /**
       * @brief - Convenience enumeration to refer to the position of
       *          each sprite type in the `m_sprites` array.
       */
      enum SpriteType {
        GroundID,
        SolidID,
        PortalID,
        EntityID,
        VFXID,
        CursorID,

        SpriteTypesCount
      };

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

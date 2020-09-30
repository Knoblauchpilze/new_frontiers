#ifndef    PGE_APP_HH
# define   PGE_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"
# include "World.hh"
# include "Controls.hh"
# include "AppDesc.hh"
# include "coordinates/CoordinateFrame.hh"
# include "ui/GameMenu.hh"

namespace new_frontiers {

  /**
   * @brief - Convenience structure defining the resources
   *          that can be displayed in any app. It contains
   *          pointers to the world's data, to the frames
   *          allowing to change from screen coordinates to
   *          world coordinates and the UI.
   */
  struct RenderDesc {
    IteratorShPtr wit;
    CoordinateFrame& cf;
    GameMenuShPtr ui;
  };

  class PGEApp: public utils::CoreObject, public olc::PixelGameEngine {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param desc - contains all the needed information to
       *               create the canvas needed by the app and
       *               set up base properties.
       */
      PGEApp(const AppDesc& desc);

      /**
       * @brief - Desctruction of the object.
       */
      ~PGEApp() = default;

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

      /**
       * @brief - Override of the destroy function which allows
       *          to release resources before the OpenGL context
       *          gets destroyed.
       * @return - `true` if the release of resources succeeded.
       */
      bool
      OnUserDestroy() override;

    protected:

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
       * @brief - Interface method allowing inheriting classes
       *          to get a chance to load resources needed for
       *          display. This method is guaranteed to be
       *          called before the first call to `draw` is
       *          issued.
       *          The default implementation does nothing.
       */
      virtual void
      loadResources();

      /**
       * @brief - Interface method allowing inheriting classes
       *          to be notified when the app is going to be
       *          destroyed so that resources can be cleaned.
       *          The default implementation does nothing.
       */
      virtual void
      cleanResources();

      bool
      isFirstFrame() const noexcept;

      bool
      hasDebug() const noexcept;

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
      redToGreenGradient(float ratio, int alpha = ALPHA_SEMI_OPAQUE) const noexcept;

      /**
       * @brief - Interface method to display the main content
       *          of the app. This method is called first and
       *          that means that it will be overriden by all
       *          UI and debug information.
       * @param res - the resources that can be drawn.
       */
      virtual void
      draw(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method allowing to draw the UI of
       *          the application. This regroups menu and all
       *          needed elements that are not game elements.
       * @param res - the resources that can be drawn.
       */
      virtual void
      drawUI(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method allowing inheriting classes
       *          to perform their own drawing calls to show
       *          debug info.
       * @param res - the resources that can be drawn.
       */
      virtual void
      drawDebug(const RenderDesc& res) = 0;

    private:

      /**
       * @brief - Convenience enumeration describing the state of the
       *          game. It allows to correctly handle transitions.
       */
      enum class State {
        Running,
        Pausing,
        Paused,
        Resuming
      };

      /**
       * @brief - Performs the initialization of the engine to make
       *          it suits our needs.
       * @param dims - a vector describing the dimensions of the
       *               canvas for this app in pixels.
       * @param pixRatio - the ratio of a pixel in the app compared
       *                   to a pixel on screen. If this value is set
       *                   set to `2` it means that each pixel in the
       *                   app's canvas will be 2x2 pixels on screen.
       */
      void
      initialize(const olc::vi2d& dims, const olc::vi2d& pixRatio);

      /**
       * @brief - Called during the initialization of the application
       *          in order to load resources needed by the menu.
       */
      void
      loadMenuResources();

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

    private:

      /**
       * @brief - Hold the representation of the in game menu for
       *          this application. The rendering and handling of
       *          the code is deferred in a dedicated class which
       *          is more robust than having everything here.
       */
      GameMenuShPtr m_menu;

      /**
       * @brief - The world managed by this application.
       */
      WorldShPtr m_world;

      /**
       * @brief - The world iterator retrieved on the world: used
       *          to traverse the elements defined in the world to
       *          display them.
       */
      IteratorShPtr m_wit;

      /**
       * @brief - Holds an object allowing to convert between the
       *          various coordinate frames handled by the app. It
       *          handles conversion between cells coordinate and
       *          screen coordinates and conversely.
       */
      CoordinateFrameShPtr m_cf;

      /**
       * @brief - A map to keep track of the state of the controls
       *          to be transmitted to the world's entities for
       *          the simulation.
       */
      Controls m_controls;

      /**
       * @brief - The index representing the main layer for this
       *          app. Given how the pixel game engine is designed
       *          we display layers with the highest order first
       *          and then in descending order.
       *          As we want the debug and UI layers to be on top
       *          of the base layer, we need to give it the largest
       *          index so that it is displayed first, and then the
       *          rest on top.
       */
      uint32_t m_mLayer;

      /**
       * @brief - The index of the layer allowing to display debug
       *          information. This layer will be assigned to the
       *          default layer created by the pixel game engine.
       *          It is needed in order to initialize the last of
       *          the layers (and thus the one that will be set on
       *          top of all the others) with meaningful data.
       */
      uint32_t m_dLayer;

      /**
       * @brief - A layer used to represent all the UI elements of
       *          the application (menu, etc).
       */
      uint32_t m_uiLayer;

      /**
       * @brief - Used to determine whether debug display is needed
       *          for this app.
       */
      bool m_debugOn;

      /**
       * @brief - Indicates that the simulation of the game is
       *          paused: the world will not evolve during this
       *          time.
       */
      State m_state;

      /**
       * @brief - Boolean allowing to display logs only on the
       *          first frame. Or do any other process a single
       *          time upon rendering the first frame.
       */
      bool m_first;
  };

}

# include "PGEApp.hxx"

#endif    /* PGE_APP_HH */

#ifndef    PGE_APP_HXX
# define   PGE_APP_HXX

# include "PGEApp.hh"

namespace new_frontiers {

  inline
  bool
  PGEApp::OnUserCreate() {
    // Create the world.
// # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100);
# endif
    m_wit = m_world->iterator();

    // Load the menu resources.
    loadMenuResources();

    // And other resources.
    loadResources();

    // The debug layer is the default layer: it is always
    // provided by the pixel game engine.
    m_dLayer = 0u;

    // Create a layer for the UI elements and enable it.
    m_uiLayer = CreateLayer();
    EnableLayer(m_uiLayer, true);

    // And finally create a layer for the main content: as
    // the pixel game engine draws layers from back to front
    // the main content should be at the back so that all
    // other elements are displayed on top of it.
    m_mLayer = CreateLayer();
    EnableLayer(m_mLayer, true);

    return true;
  }

  inline
  bool
  PGEApp::OnUserUpdate(float fElapsedTime) {
    // Handle inputs.
    bool r = handleInputs();

    // Handle game logic if needed.
    switch (m_state) {
      case State::Running:
        m_world->step(fElapsedTime, m_controls);
        break;
      case State::Pausing:
        m_world->pause(fElapsedTime, m_controls);
        m_state = State::Paused;
        break;
      case State::Resuming:
        m_world->resume(fElapsedTime, m_controls);
        m_state = State::Running;
        break;
      case State::Paused:
      default:
        break;
    }

    // Handle menus update.
    m_menu->processUserInput(m_controls);

    // Handle rendering: for each function
    // we will assign the draw target first
    // so that the function does not have
    // to handle it. We want to save the
    // current draw target to restore it
    // once the process is done.
    olc::Sprite* base = GetDrawTarget();

    RenderDesc res{
      m_wit,
      *m_cf,
      m_menu
    };

    SetDrawTarget(m_mLayer);
    draw(res);

    SetDrawTarget(m_uiLayer);
    drawUI(res);

    // Note that as the debug layer is the
    // layer `0`, we always have to display
    // it, we just need to not draw anything
    // if the debug mode is deactivated.
    SetDrawTarget(m_dLayer);
    drawDebug(res);

    // Restore the target.
    SetDrawTarget(base);

    // Not the first frame anymore.
    m_first = false;

    return r;
  }

  inline
  bool
  PGEApp::OnUserDestroy() {
    // Clear menu resources.
    if (m_menu != nullptr) {
      m_menu.reset();
    }

    // And other resources.
    cleanResources();

    return true;
  }

  inline
  void
  PGEApp::loadResources() {}

  inline
  void
  PGEApp::cleanResources() {}

  inline
  bool
  PGEApp::isFirstFrame() const noexcept {
    return m_first;
  }

  inline
  bool
  PGEApp::hasDebug() const noexcept {
    return m_debugOn;
  }
  inline
  olc::Pixel
  PGEApp::redToGreenGradient(float ratio, int alpha) const noexcept {
    // Go from red to orange to yellow to green
    // based on the value of the ratio. It fits
    // nicely in the range `[0; 1]` where each
    // `0.33` we have a new color.
    ratio = std::min(std::max(ratio, 0.0f), 1.0f);

    float t = 0.0f;
    olc::Pixel l, h;

    if (ratio <= 0.33f) {
      // Red to orange.
      t = ratio / 0.33f;

      l = olc::Pixel(255u, 0u, 0u);
      h = olc::Pixel(255u, 128u, 0u);
    }
    else if (ratio <= 0.66f) {
      // Orange to yellow.
      t = (ratio - 0.33f) / 0.33f;

      l = olc::Pixel(255u, 128u, 0u);
      h = olc::Pixel(255u, 255u, 0u);
    }
    else {
      // Yellow to green
      t = (ratio - 0.66f) / 0.34f;

      l = olc::Pixel(255u, 255u, 0u);
      h = olc::Pixel(0u, 255u, 0u);
    }

    return olc::Pixel(
      static_cast<int>((1.0f - t) * l.r + t * h.r),
      static_cast<int>((1.0f - t) * l.g + t * h.g),
      static_cast<int>((1.0f - t) * l.b + t * h.b),
      alpha
    );
  }

  inline
  void
  PGEApp::initialize(const olc::vi2d& dims, const olc::vi2d& pixRatio) {
    // Construct the window. Note that we use a pixel size
    // to screen size ratio of `1` (meaning that each pixel
    // of the viewport will be represented by a pixel on
    // the screen).
    olc::rcode c = Construct(dims.x, dims.y, pixRatio.x, pixRatio.y);

    if (c != olc::OK) {
      throw utils::CoreException(
        std::string("Could not build new frontiers application"),
        std::string("Initialization failed")
      );
    }
  }

}

#endif    /* PGE_APP_HXX */

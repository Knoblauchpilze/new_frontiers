#ifndef    PGE_APP_HXX
# define   PGE_APP_HXX

# include "PGEApp.hh"
# include "ui/Action.hh"

namespace new_frontiers {

  inline
  bool
  PGEApp::OnUserUpdate(float fElapsedTime) {
    // Handle inputs.
    InputChanges ic = handleInputs();

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

    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    m_menu->processUserInput(m_controls, actions);

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_world);
    }

    // Handle rendering: for each function
    // we will assign the draw target first
    // so that the function does not have
    // to handle it. We want to save the
    // current draw target to restore it
    // once the process is done.
    olc::Sprite* base = GetDrawTarget();

    RenderDesc res{
      m_loc,
      *m_cf,
      m_menu
    };

    SetDrawTarget(m_mLayer);
    draw(res);

    if (hasUI()) {
      SetDrawTarget(m_uiLayer);
      drawUI(res);
    }
    if (!hasUI() && isFirstFrame()) {
      SetDrawTarget(m_uiLayer);
      clearLayer(res);
    }

    // Draw the debug layer. As it is saved
    // in the layer `0` we need to clear it
    // when it is not displayed as it will
    // be rendered on top of any other layer
    // and thus we would still display the
    // last frame when it is inactive.
    // Note that we also clear it in case
    // the debug is set to `false` from the
    // beginning of the rendering: if we
    // don't do this nothing will be visible
    // as the `0`-th layer would never be
    // updated.
    if (hasDebug()) {
      SetDrawTarget(m_dLayer);
      drawDebug(res);
    }
    if (!hasDebug() && (ic.debugLayerToggled || isFirstFrame())) {
      SetDrawTarget(m_dLayer);
      clearLayer(res);
    }

    // Restore the target.
    SetDrawTarget(base);

    // Not the first frame anymore.
    m_first = false;

    return !ic.quit;
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
  bool
  PGEApp::hasUI() const noexcept {
    return m_uiOn;
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
  PGEApp::drawHealthBar(const SpriteDesc& tile,
                        float ratio,
                        const CoordinateFrame& cf,
                        const Orientation& o)
  {
    // Fetch a color based on the input ratio.
    olc::Pixel hbc = redToGreenGradient(ratio, tile.alpha);

    // Darken it for the empty portion of the
    // healthbar.
    olc::Pixel bc(
      static_cast<int>(hbc.r * 0.5f),
      static_cast<int>(hbc.g * 0.5f),
      static_cast<int>(hbc.b * 0.5f),
      hbc.a
    );

    // Draw both parts of the healthbar. The
    // combined length of both elements will
    // be placed close to the position of the
    // actual element and have a size of 7/10th
    // of a tile in width, and 1/10th of a tile
    // in height.
    float hbWRatio = 0.7f;
    float hbHRatio = 0.1f;
    float hbHOffset = 0.1f;
    olc::vf2d s = tile.radius * cf.tileSize();

    if (o == Orientation::Horizontal) {
      olc::vf2d p = cf.tileCoordsToPixels(tile.x, tile.y, tile.radius, tile.location);

      FillRectDecal(
        olc::vf2d(p.x + (1.0f - hbWRatio) * s.x / 2.0f, p.y - s.y * hbHOffset),
        olc::vf2d(s.x * hbWRatio * ratio, s.y * hbHRatio),
        hbc
      );
      FillRectDecal(
        olc::vf2d(p.x + (1.0f - hbWRatio) * s.x / 2.0f + s.x * hbWRatio * ratio, p.y - s.y * hbHOffset),
        olc::vf2d(s.x * hbWRatio * (1.0f - ratio), s.y * hbHRatio),
        bc
      );
    }

    if (o == Orientation::Vertical) {
      olc::vf2d p = cf.tileCoordsToPixels(tile.x, tile.y, tile.radius, tile.location);

      FillRectDecal(
        olc::vf2d(p.x + s.x * hbHOffset, p.y - s.y * hbWRatio),
        olc::vf2d(s.x * hbHRatio, s.y * hbWRatio * (1.0f - ratio)),
        bc
      );

      FillRectDecal(
        olc::vf2d(p.x + s.x * hbHOffset, p.y - s.y * hbWRatio * ratio),
        olc::vf2d(s.x * hbHRatio, s.y * hbWRatio * ratio),
        hbc
      );
    }
  }

  inline
  void
  PGEApp::clearLayer(const RenderDesc& /*res*/) {
    // Clear the canvas with a neutral fully transparent color.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));
    SetPixelMode(olc::Pixel::NORMAL);
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

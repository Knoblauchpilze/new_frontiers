
# include "PGEApp.hh"
# include "utils.hh"
# include "Controls.hh"

namespace new_frontiers {

  PGEApp::PGEApp(const AppDesc& desc):
    utils::CoreObject(desc.name),
    olc::PixelGameEngine(),

    m_menu(nullptr),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(desc.frame),

    m_controls(newControls()),

    m_mLayer(0u),
    m_dLayer(0u),
    m_uiLayer(0u),

    m_debugOn(false),
    m_uiOn(true),

    m_state(State::Running),
    m_first(true)
  {
    // Initialize the application settings.
    sAppName = desc.name;
    setService("app");

    // Make sure that a coordinate frame is provided.
    if (m_cf == nullptr) {
      error(
        std::string("Unable to create PGE application"),
        std::string("Invalid null coordinate frame provided")
      );
    }

    // Generate and construct the window.
    initialize(desc.dims, desc.pixRatio);
  }

  void
  PGEApp::loadMenuResources() {
    // Create the menu: we want it to have a fixed
    // height and be at the bottom of the screen.
    int mHeight = 150;

    int w = ScreenWidth();
    int h = ScreenHeight();

    olc::vi2d mPos(0, h - mHeight);
    olc::vf2d mSize(w, mHeight);

    m_menu = std::make_shared<GameMenu>(mPos, mSize);
  }

  PGEApp::InputChanges
  PGEApp::handleInputs() {
    InputChanges ic{false, false};

    // Detect press on `Escape` key to shutdown the app.
    olc::HWButton esc = GetKey(olc::ESCAPE);
    if (esc.bReleased) {
      ic.quit = true;
      return ic;
    }

    // In case we're dragging the right mouse button we
    // will update the world's position (panning). What
    // we want is to remember the position at the moment
    // of the click and then continuously move the world
    // to match the current displacement.
    if (GetMouse(1).bPressed) {
      m_cf->beginTranslation(GetMousePos());
    }
    if (GetMouse(1).bHeld) {
      m_cf->translate(GetMousePos());
    }

    olc::vi2d mPos = GetMousePos();
    m_controls.mPosX = mPos.x;
    m_controls.mPosY = mPos.y;

    // Detect clicks with the left mouse button to be
    // selecting game elements.
    if (GetMouse(0).bReleased) {
      log("Should detect element", utils::Level::Error);
    }

    int scroll = GetMouseWheel();
    if (scroll > 0) {
      m_cf->zoomIn(GetMousePos());
    }
    if (scroll < 0) {
      m_cf->zoomOut(GetMousePos());
    }

    // Handle inputs.
    olc::HWButton b = GetKey(olc::RIGHT);
    m_controls.keys[Right] = b.bPressed || b.bHeld;

    b = GetKey(olc::UP);
    m_controls.keys[Up] = b.bPressed || b.bHeld;

    b = GetKey(olc::LEFT);
    m_controls.keys[Left] = b.bPressed || b.bHeld;

    b = GetKey(olc::DOWN);
    m_controls.keys[Down] = b.bPressed || b.bHeld;

    b = GetKey(olc::SPACE);
    m_controls.keys[Space] = b.bPressed || b.bHeld;

    // De/activate the debug mode if needed and
    // handle general simulation control options.
    if (GetKey(olc::D).bReleased) {
      m_debugOn = !m_debugOn;
      ic.debugLayerToggled = true;
    }
    if (GetKey(olc::U).bReleased) {
      m_uiOn = !m_uiOn;
    }

    if (GetKey(olc::P).bReleased) {
      switch (m_state) {
        case State::Running:
        case State::Resuming:
          m_state = State::Pausing;
          break;
        case State::Paused:
          m_state = State::Resuming;
          break;
        case State::Pausing:
        default:
          break;
      }
    }

    return ic;
  }

}

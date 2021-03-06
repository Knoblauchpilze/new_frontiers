
# include "PGEApp.hh"
# include "utils.hh"
# include "Controls.hh"

namespace new_frontiers {

  PGEApp::PGEApp(const AppDesc& desc):
    utils::CoreObject(desc.name),
    olc::PixelGameEngine(),

    m_menu(nullptr),

    m_world(nullptr),
    m_loc(nullptr),

    m_cf(desc.frame),

    m_controls(controls::newState()),

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

  bool
  PGEApp::OnUserCreate() {
    // Create the world.
// # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100, 15, 15);
# endif
    m_loc = m_world->locator();

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

  void
  PGEApp::loadMenuResources() {
    // Create the menu: we want it to have a fixed
    // height and be at the bottom of the screen.
    int w = ScreenWidth();
    int h = ScreenHeight();

    olc::vi2d mPos(0, h - MENU_HEIGHT);
    olc::vf2d mSize(w, MENU_HEIGHT);

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

    int scroll = GetMouseWheel();
    if (scroll > 0) {
      m_cf->zoomIn(GetMousePos());
    }
    if (scroll < 0) {
      m_cf->zoomOut(GetMousePos());
    }

    // Handle inputs.
    olc::HWButton b = GetKey(olc::RIGHT);
    m_controls.keys[controls::keys::Right] = b.bPressed || b.bHeld;

    b = GetKey(olc::UP);
    m_controls.keys[controls::keys::Up] = b.bPressed || b.bHeld;

    b = GetKey(olc::LEFT);
    m_controls.keys[controls::keys::Left] = b.bPressed || b.bHeld;

    b = GetKey(olc::DOWN);
    m_controls.keys[controls::keys::Down] = b.bPressed || b.bHeld;

    b = GetKey(olc::SPACE);
    m_controls.keys[controls::keys::Space] = b.bPressed || b.bHeld;

    b = GetKey(olc::TAB),
    m_controls.tab = b.bReleased;

    auto analysis = [](const olc::HWButton& b) {
      if (b.bPressed) {
        return controls::ButtonState::Pressed;
      }
      if (b.bHeld) {
        return controls::ButtonState::Held;
      }
      if (b.bReleased) {
        return controls::ButtonState::Released;
      }

      return controls::ButtonState::Free;
    };

    m_controls.buttons[controls::mouse::Left] = analysis(GetMouse(0));
    m_controls.buttons[controls::mouse::Right] = analysis(GetMouse(1));
    m_controls.buttons[controls::mouse::Middle] = analysis(GetMouse(2));

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

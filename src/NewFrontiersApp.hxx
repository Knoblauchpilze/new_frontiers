#ifndef    NEW_FRONTIERS_APP_HXX
# define   NEW_FRONTIERS_APP_HXX

# include "NewFrontiersApp.hh"

namespace new_frontiers {

  inline
  NewFrontiersApp::~NewFrontiersApp() {
    if (m_sprite != nullptr) {
      delete m_sprite;
    }
  }

  inline
  bool
  NewFrontiersApp::OnUserCreate() {
    // Create the world.
    m_world = std::make_shared<World>(100, 8, 6);
    m_wit = m_world->iterator();

    // And the tile alias.
    createTileAliases();

    return true;
  }

  inline
  bool
  NewFrontiersApp::OnUserUpdate(float fElapsedTime) {
    // Handle inputs.
    bool r = handleInputs();

    // Handle game logic.
    m_world->step(fElapsedTime);

    // Handle rendering.
    draw();

    return r;
  }

  inline
  void
  NewFrontiersApp::initialize(int width, int height, int pixelRatio) {
    // Construct the window. Note that we use a pixel size
    // to screen size ratio of `1` (meaning that each pixel
    // of the viewport will be represented by a pixel on
    // the screen).
    olc::rcode c = Construct(width, height, pixelRatio, pixelRatio);

    if (c != olc::OK) {
      throw utils::CoreException(
        std::string("Could not build new frontiers application"),
        std::string("Initialization failed")
      );
    }
  }

  inline
  olc::vi2d
  NewFrontiersApp::spriteCoordsToPixels(int x, int y) const noexcept {
    return olc::vi2d(x * m_ss.x, y * m_ss.y);
  }

  inline
  int
  NewFrontiersApp::aliasOfSprite(const Sprite& sprite) {
    return sprite;
  }

  inline
  int
  NewFrontiersApp::aliasOfEntity(const Mob& ent) {
    return SpritesCount + ent;
  }

  inline
  int
  NewFrontiersApp::aliasOfEffect(const Effect& vfx) {
    return SpritesCount + MobsCount + vfx;
  }

  inline
  void
  NewFrontiersApp::drawSprite(int x, int y, int alias) {
    DrawPartialDecal(m_cf.tileCoordsToPixels(x, y), m_sprite, m_aliases[alias], m_ss);
  }

}

#endif    /* NEW_FRONTIERS_APP_HXX */

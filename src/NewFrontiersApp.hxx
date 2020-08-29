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
    m_world = std::make_shared<World>(10, 8, 6);
    m_wit = m_world->iterator();

    // And the tile alias.
    createTileAliases();

    return true;
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

}

#endif    /* NEW_FRONTIERS_APP_HXX */

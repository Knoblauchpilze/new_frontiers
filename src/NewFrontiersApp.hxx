#ifndef    NEW_FRONTIERS_APP_HXX
# define   NEW_FRONTIERS_APP_HXX

# include "NewFrontiersApp.hh"

namespace new_frontiers {

  inline
  NewFrontiersApp::~NewFrontiersApp() {
    if (!m_sprites.empty()) {
      for (unsigned id = 0u ; id < m_sprites.size() ; ++id) {
        delete m_sprites[id].res;
      }

      m_sprites.clear();
    }
  }

  inline
  bool
  NewFrontiersApp::OnUserCreate() {
    // Create the world.
    // m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
    m_world = std::make_shared<World>(100);
    m_wit = m_world->iterator();

    // And the tile alias.
    createTileAliases();

    // The debug layer is the default layer: it is always
    // provided by the pixel game engine.
    m_dLayer = 0u;

    // Create a layer for the main content and enable it.
    m_mLayer = CreateLayer();
    EnableLayer(m_mLayer, true);

    return true;
  }

  inline
  bool
  NewFrontiersApp::OnUserUpdate(float fElapsedTime) {
    // Handle inputs.
    bool r = handleInputs();

    // Handle game logic.
    m_world->step(fElapsedTime, m_controls);

    // Handle rendering: for each function
    // we will assign the draw target first
    // so that the function does not have
    // to handle it. We want to save the
    // current draw target to restore it
    // once the process is done.
    olc::Sprite* base = GetDrawTarget();

    SetDrawTarget(m_mLayer);
    draw();

    SetDrawTarget(m_dLayer);
    drawDebug();

    // Restore the target.
    SetDrawTarget(base);

    return r;
  }

  inline
  void
  NewFrontiersApp::initialize(int width, int height, const Theme& theme, int pixelRatio) {
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

    // Create the placeholder for sprites and resource packs.
    m_sprites.resize(SpriteTypesCount);

    m_sprites[SolidID].file = theme.solidTiles.file;
    m_sprites[SolidID].layout = theme.solidTiles.layout;
    m_sprites[SolidID].res = nullptr;

    m_sprites[PortalID].file = theme.portals.file;
    m_sprites[PortalID].layout = theme.portals.layout;
    m_sprites[PortalID].res = nullptr;

    m_sprites[EntityID].file = theme.entities.file;
    m_sprites[EntityID].layout = theme.entities.layout;
    m_sprites[EntityID].res = nullptr;

    m_sprites[VFXID].file = theme.vfx.file;
    m_sprites[VFXID].layout = theme.vfx.layout;
    m_sprites[VFXID].res = nullptr;

    m_sprites[CursorID].file = theme.cursors.file;
    m_sprites[CursorID].layout = theme.cursors.layout;
    m_sprites[CursorID].res = nullptr;
  }

  inline
  olc::vi2d
  NewFrontiersApp::spriteCoordsToPixels(const olc::vi2d& coord, const olc::vi2d& layout, int id) const noexcept {
    // Compute linear identifier from 2D coordinates.
    int lID = coord.y * layout.x + coord.x + id;

    // Go back to 2D coordinates using the layout on
    // the linearized ID and the size of the sprite
    // to obtain a pixels position.
    return olc::vi2d(
      (lID % layout.x) * m_ss.x,
      (lID / layout.x) * m_ss.y
    );
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

// # define DRAW_DECAL

  inline
  void
# ifdef DRAW_DECAL
  NewFrontiersApp::drawSprite(float x, float y, int alias, int id, int alpha) {
# else
  NewFrontiersApp::drawSprite(float x, float y, int alias, int id, int /*alpha*/) {
# endif
    const SpriteAlias& sa = m_aliases[alias];
    const SpritesPack& sp = m_sprites[sa.type];

# ifdef DRAW_DECAL
    DrawPartialDecal(
      m_cf.tileCoordsToPixels(x, y),
      sp.res,
      spriteCoordsToPixels(sa.alias, sp.layout, id),
      m_ss,
      m_cf.tileScale(),
      olc::Pixel(255, 255, 255, alpha)
    );
# else
    DrawPartialSprite(
      m_cf.tileCoordsToPixels(x, y),
      sp.res->sprite,
      spriteCoordsToPixels(sa.alias, sp.layout, id),
      m_ss
    );
# endif
  }

}

#endif    /* NEW_FRONTIERS_APP_HXX */

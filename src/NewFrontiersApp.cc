
# include "NewFrontiersApp.hh"
# include "utils.hh"

namespace new_frontiers {

  NewFrontiersApp::NewFrontiersApp(int width,
                                   int height,
                                   const Theme& theme,
                                   int pixelRatio,
                                   const std::string& appName):
    utils::CoreObject(appName),
    olc::PixelGameEngine(),

    m_theme(theme),
    m_sprite(nullptr),
    m_aliases(),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(10.0f, 15.0f)},
         Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(width, height)},
         olc::vi2d(64, 32)),

    m_first(true)
  {

    // Initialize the application settings.
    sAppName = appName;
    setService("app");

    // Generate and construct the window.
    initialize(width, height, pixelRatio);
  }

  void
  NewFrontiersApp::createTileAliases() {
    // Load the sprite containing textures as
    // defined by the theme.
    olc::Sprite* spr = new olc::Sprite(m_theme.file);
    m_sprite = new olc::Decal(spr);

    // Build the atlas.
    int spritesCount = SpritesCount + MobsCount + EffectsCount;
    m_aliases.resize(spritesCount + 1);

    m_aliases[aliasOfSprite(Empty)]         = olc::vi2d(4, 2);
    m_aliases[aliasOfSprite(Wall)]          = olc::vi2d(0, 0);
    m_aliases[aliasOfSprite(Door)]          = olc::vi2d(0, 2);
    m_aliases[aliasOfEntity(Knight)]        = olc::vi2d(4, 7);
    m_aliases[aliasOfEntity(Snake)]         = olc::vi2d(5, 7);
    m_aliases[aliasOfEntity(Warlord)]       = olc::vi2d(6, 7);
    m_aliases[aliasOfEntity(Vampire)]       = olc::vi2d(7, 7);
    m_aliases[aliasOfEntity(Princess)]      = olc::vi2d(8, 7);
    m_aliases[aliasOfEntity(Sphynx)]        = olc::vi2d(9, 7);
    m_aliases[aliasOfEffect(Fire)]          = olc::vi2d(8, 10);
    m_aliases[aliasOfEffect(Electricity)]   = olc::vi2d(11, 10);
    m_aliases[aliasOfEffect(Gas)]           = olc::vi2d(14, 10);
    m_aliases[aliasOfEffect(Smoke)]         = olc::vi2d(1, 11);

    m_aliases[spritesCount] = olc::vi2d(5, 10);
  }

  bool
  NewFrontiersApp::handleInputs() {
    // Detect press on `Escape` key to shutdown the app.
    olc::HWButton esc = GetKey(olc::ESCAPE);
    if (esc.bReleased) {
      return false;
    }

    // In case we're dragging the right mouse button we
    // will update the world's position (panning). What
    // we want is to remember the position at the moment
    // of the click and then continuously move the world
    // to match the current displacement.
    if (GetMouse(1).bPressed) {
      m_cf.beginTranslation(GetMousePos());
    }
    if (GetMouse(1).bHeld) {
      m_cf.translate(GetMousePos());
    }

    int scroll = GetMouseWheel();
    if (scroll > 0) {
      m_cf.zoom(Zoom::In, GetMousePos());
    }
    if (scroll < 0) {
      m_cf.zoom(Zoom::Out, GetMousePos());
    }

    return true;
  }

  void
  NewFrontiersApp::draw() {
    // Clear rendering target.
    Clear(olc::VERY_DARK_GREEN);
    SetPixelMode(olc::Pixel::ALPHA);

    // Drax elements of the world.

    // Draw ground.
    for (int y = 0 ; y < m_wit->h() ; ++y) {
      for (int x = 0 ; x < m_wit->w() ; ++x) {
        drawSprite(x, y, Sprite::Empty, 0);
      }
    }

    // Draw solid tiles.
    for (int id = 0 ; id < m_wit->solidTilesCount() ; ++id) {
      SolidTile t = m_wit->solidTile(id);
      drawSprite(t.x, t.y, aliasOfSprite(t.type), t.id);
    }

    // Draw entities.
    for (int id = 0 ; id < m_wit->entitiesCount() ; ++id) {
      EntityTile t = m_wit->entity(id);
      drawSprite(t.x, t.y, aliasOfEntity(t.type), t.id);
    }

    // Draw vfx.
    for (int id = 0 ; id < m_wit->vfxCount() ; ++id) {
      VFXTile t = m_wit->vfx(id);
      drawSprite(t.x, t.y, aliasOfEffect(t.type), t.id);
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp);
    drawSprite(mtp.x, mtp.y, m_aliases.size() - 1, 0);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  NewFrontiersApp::drawDebug() {
    // Render mouse and world cell coordinates.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp);

    DrawString(olc::vi2d(0, 450), "Mouse coords        : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 465), "World cell coords   : " + toString(mtp), olc::CYAN);

    // Not the first frame anymore.
    m_first = false;
  }

}

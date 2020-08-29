
# include "NewFrontiersApp.hh"
# include "utils.hh"

namespace new_frontiers {

  NewFrontiersApp::NewFrontiersApp(int width,
                                   int height,
                                   int pixelRatio,
                                   const std::string& appName):
    utils::CoreObject(appName),
    olc::PixelGameEngine(),

    m_sprite(nullptr),
    m_ss(64, 64),
    m_aliases(),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(10.0f, 15.0f)},
         Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(width, height)},
         olc::vi2d(64, 32))
  {

    // Initialize the application settings.
    sAppName = appName;
    setService("app");

    // Generate and construct the window.
    initialize(width, height, pixelRatio);
  }

  void
  NewFrontiersApp::createTileAliases() {
    // Load the sprite containing textures.
    olc::Sprite* spr = new olc::Sprite("data/img/64x64.png");
    m_sprite = new olc::Decal(spr);

    // Build the atlas.
    int spritesCount = SpritesCount + MobsCount + EffectsCount;
    m_aliases.resize(spritesCount + 1);

    m_aliases[aliasOfSprite(Empty)]         = spriteCoordsToPixels(10, 4);
    m_aliases[aliasOfSprite(Wall_Dirt)]     = spriteCoordsToPixels(1, 0);
    m_aliases[aliasOfSprite(Wall_Stone)]    = spriteCoordsToPixels(1, 1);
    m_aliases[aliasOfSprite(Wall_Catacomb)] = spriteCoordsToPixels(3, 2);
    m_aliases[aliasOfSprite(Wall_Slime)]    = spriteCoordsToPixels(3, 3);
    m_aliases[aliasOfSprite(Well)]          = spriteCoordsToPixels(3, 4);
    m_aliases[aliasOfSprite(Statue)]        = spriteCoordsToPixels(4, 4);
    m_aliases[aliasOfSprite(Fluid)]         = spriteCoordsToPixels(7, 4);
    m_aliases[aliasOfSprite(Portal)]        = spriteCoordsToPixels(11, 5);
    m_aliases[aliasOfEntity(Knight)]        = spriteCoordsToPixels(4, 7);
    m_aliases[aliasOfEntity(Snake)]         = spriteCoordsToPixels(5, 7);
    m_aliases[aliasOfEntity(Warlord)]       = spriteCoordsToPixels(6, 7);
    m_aliases[aliasOfEntity(Vampire)]       = spriteCoordsToPixels(7, 7);
    m_aliases[aliasOfEntity(Princess)]      = spriteCoordsToPixels(8, 7);
    m_aliases[aliasOfEntity(Sphynx)]        = spriteCoordsToPixels(9, 7);
    m_aliases[aliasOfEffect(Fire)]          = spriteCoordsToPixels(8, 10);
    m_aliases[aliasOfEffect(Electricity)]   = spriteCoordsToPixels(11, 10);
    m_aliases[aliasOfEffect(Gas)]           = spriteCoordsToPixels(14, 10);
    m_aliases[aliasOfEffect(Smoke)]         = spriteCoordsToPixels(1, 11);

    m_aliases[spritesCount] = spriteCoordsToPixels(5, 10);
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
        drawSprite(x, y, Sprite::Empty);
      }
    }

    // Draw solid tiles.
    for (int id = 0 ; id < m_wit->solidTilesCount() ; ++id) {
      SolidTile t = m_wit->solidTile(id);
      drawSprite(t.x, t.y, aliasOfSprite(t.type));
    }

    // Draw entities.
    for (int id = 0 ; id < m_wit->entitiesCount() ; ++id) {
      EntityTile t = m_wit->entity(id);
      drawSprite(t.x, t.y, aliasOfEntity(t.type));
    }

    // Draw vfx.
    for (int id = 0 ; id < m_wit->vfxCount() ; ++id) {
      VFXTile t = m_wit->vfx(id);
      drawSprite(t.x, t.y, aliasOfEffect(t.type));
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    int q;
    olc::vi2d gc;
    olc::vf2d to;
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp, q, gc, to);
    drawSprite(mtp.x, mtp.y, m_aliases.size() - 1);

    SetPixelMode(olc::Pixel::NORMAL);

    DrawString(olc::vi2d(0, 405), "q                   : " + std::to_string(q), olc::CYAN);
    DrawString(olc::vi2d(0, 420), "Grid cells          : " + toString(gc), olc::CYAN);
    DrawString(olc::vi2d(0, 435), "Cell offset         : " + toString(to), olc::CYAN);
    DrawString(olc::vi2d(0, 450), "Mouse coords        : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 465), "World cell coords   : " + toString(mtp), olc::CYAN);
  }

}

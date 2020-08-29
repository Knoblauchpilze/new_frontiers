
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
    m_aliases(),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(olc::vi2d(5, 1))
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
    m_sprite = new olc::Sprite("data/img/64x64.png");

    // Build the atlas.
    int spritesCount = SpritesCount + MobsCount + EffectsCount;
    m_aliases.resize(spritesCount + 1);

    m_aliases[aliasOfSprite(Empty)]         = m_cf.spriteCoordsToPixels(10, 4);
    m_aliases[aliasOfSprite(Wall_Dirt)]     = m_cf.spriteCoordsToPixels(1, 0);
    m_aliases[aliasOfSprite(Wall_Stone)]    = m_cf.spriteCoordsToPixels(1, 1);
    m_aliases[aliasOfSprite(Wall_Catacomb)] = m_cf.spriteCoordsToPixels(3, 2);
    m_aliases[aliasOfSprite(Wall_Slime)]    = m_cf.spriteCoordsToPixels(3, 3);
    m_aliases[aliasOfSprite(Well)]          = m_cf.spriteCoordsToPixels(3, 4);
    m_aliases[aliasOfSprite(Statue)]        = m_cf.spriteCoordsToPixels(4, 4);
    m_aliases[aliasOfSprite(Fluid)]         = m_cf.spriteCoordsToPixels(7, 4);
    m_aliases[aliasOfSprite(Portal)]        = m_cf.spriteCoordsToPixels(11, 5);
    m_aliases[aliasOfEntity(Knight)]        = m_cf.spriteCoordsToPixels(4, 7);
    m_aliases[aliasOfEntity(Snake)]         = m_cf.spriteCoordsToPixels(5, 7);
    m_aliases[aliasOfEntity(Warlord)]       = m_cf.spriteCoordsToPixels(6, 7);
    m_aliases[aliasOfEntity(Vampire)]       = m_cf.spriteCoordsToPixels(7, 7);
    m_aliases[aliasOfEntity(Princess)]      = m_cf.spriteCoordsToPixels(8, 7);
    m_aliases[aliasOfEntity(Sphynx)]        = m_cf.spriteCoordsToPixels(9, 7);
    m_aliases[aliasOfEffect(Fire)]          = m_cf.spriteCoordsToPixels(8, 10);
    m_aliases[aliasOfEffect(Electricity)]   = m_cf.spriteCoordsToPixels(11, 10);
    m_aliases[aliasOfEffect(Gas)]           = m_cf.spriteCoordsToPixels(14, 10);
    m_aliases[aliasOfEffect(Smoke)]         = m_cf.spriteCoordsToPixels(1, 11);

    m_aliases[spritesCount] = m_cf.spriteCoordsToPixels(5, 10);
  }

  bool
  NewFrontiersApp::handleInputs() {
    // Detect press on `Escape` key to shutdown the app.
    olc::HWButton esc = GetKey(olc::ESCAPE);
    if (esc.bReleased) {
      return false;
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
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp);
    drawSprite(mtp.x, mtp.y, m_aliases.size() - 1);

    SetPixelMode(olc::Pixel::NORMAL);

    DrawString(olc::vi2d(0, 450), "Mouse coords        : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 465), "World cell coords   : " + toString(mtp), olc::CYAN);
  }

}

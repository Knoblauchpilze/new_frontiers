
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

    m_wox(5),
    m_woy(1),

    m_world(nullptr)
  {

    // Initialize the application settings.
    sAppName = appName;
    setService("app");

    // Generate and construct the window.
    initialize(width, height, pixelRatio);
  }

  inline
  void
  NewFrontiersApp::drawSprite(int x, int y, int alias) {
    DrawPartialSprite(tileCoordsToPixels(x, y), m_sprite, m_aliases[alias], spriteSize());
  }

  bool
  NewFrontiersApp::OnUserUpdate(float /*fElapsedTime*/) {
    // Clear rendering target.
    Clear(olc::VERY_DARK_GREEN);
    SetPixelMode(olc::Pixel::ALPHA);

    // Draw the world.
    WorldIterator it = m_world->iterator();

    // Draw ground.
    for (int y = 0 ; y < it.h() ; ++y) {
      for (int x = 0 ; x < it.w() ; ++x) {
        drawSprite(x, y, Sprite::Empty);
      }
    }

    // Draw solid tiles.
    for (int id = 0 ; id < it.solidTilesCount() ; ++id) {
      SolidTile t = it.solidTile(id);
      drawSprite(t.x, t.y, aliasOfSprite(t.type));
    }

    // Draw entities.
    for (int id = 0 ; id < it.entitiesCount() ; ++id) {
      EntityTile t = it.entity(id);
      drawSprite(t.x, t.y, aliasOfEntity(t.type));
    }

    // Draw vfx.
    for (int id = 0 ; id < it.vfxCount() ; ++id) {
      VFXTile t = it.vfx(id);
      drawSprite(t.x, t.y, aliasOfEffect(t.type));
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = pixelCoordsToTiles(mp);
    drawSprite(mtp.x, mtp.y, m_aliases.size() - 1);

    SetPixelMode(olc::Pixel::NORMAL);

    DrawString(olc::vi2d(0, 450), "Mouse coords        : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 465), "World cell coords   : " + toString(mtp), olc::CYAN);

    return true;
  }

  void
  NewFrontiersApp::createTileAliases() {
    // Load the sprite containing textures.
    m_sprite = new olc::Sprite("data/img/64x64.png");

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

}

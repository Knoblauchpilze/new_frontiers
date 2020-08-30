
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

    m_ss(theme.size),
    m_sprites(),
    m_aliases(),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(10.0f, 15.0f)},
         Viewport{olc::vf2d(320.0f, 100.0f), olc::vf2d(width, height)},
         olc::vi2d(64, 32)),

    m_first(true)
  {

    // Initialize the application settings.
    sAppName = appName;
    setService("app");

    // Generate and construct the window.
    initialize(width, height, theme, pixelRatio);
  }

  void
  NewFrontiersApp::createTileAliases() {
    // Load the sprites containing textures as
    // defined by the theme.
    m_sprites.resize(SpriteTypesCount);

    olc::Sprite* spr = new olc::Sprite(m_sprites[SolidID].file);
    m_sprites[SolidID].res = new olc::Decal(spr);

    spr = new olc::Sprite(m_sprites[PortalID].file);
    m_sprites[PortalID].res = new olc::Decal(spr);

    spr = new olc::Sprite(m_sprites[EntityID].file);
    m_sprites[EntityID].res = new olc::Decal(spr);

    spr = new olc::Sprite(m_sprites[VFXID].file);
    m_sprites[VFXID].res = new olc::Decal(spr);

    spr = new olc::Sprite(m_sprites[CursorID].file);
    m_sprites[CursorID].res = new olc::Decal(spr);

    // Build the atlas.
    int spritesCount = SpritesCount + MobsCount + EffectsCount;
    m_aliases.resize(spritesCount + 1);

    m_aliases[aliasOfSprite(Empty)]         = SpriteAlias{SolidID, olc::vi2d(4, 2)};
    m_aliases[aliasOfSprite(Wall)]          = SpriteAlias{SolidID, olc::vi2d(0, 0)};
    m_aliases[aliasOfSprite(Door)]          = SpriteAlias{SolidID, olc::vi2d(0, 2)};

    m_aliases[aliasOfSprite(Portal)]        = SpriteAlias{PortalID, olc::vi2d(0, 0)};

    m_aliases[aliasOfEntity(MarineKnight)]      = SpriteAlias{EntityID, olc::vi2d(0, 0)};
    m_aliases[aliasOfEntity(Warrior)]           = SpriteAlias{EntityID, olc::vi2d(1, 0)};
    m_aliases[aliasOfEntity(StoneGiant)]        = SpriteAlias{EntityID, olc::vi2d(2, 0)};
    m_aliases[aliasOfEntity(Sorceress)]         = SpriteAlias{EntityID, olc::vi2d(3, 0)};
    m_aliases[aliasOfEntity(Knight)]            = SpriteAlias{EntityID, olc::vi2d(4, 0)};
    m_aliases[aliasOfEntity(GeneticExperiment)] = SpriteAlias{EntityID, olc::vi2d(5, 0)};
    m_aliases[aliasOfEntity(Warlord)]           = SpriteAlias{EntityID, olc::vi2d(6, 0)};
    m_aliases[aliasOfEntity(Vampire)]           = SpriteAlias{EntityID, olc::vi2d(7, 0)};
    m_aliases[aliasOfEntity(Gecko)]             = SpriteAlias{EntityID, olc::vi2d(9, 0)};
    m_aliases[aliasOfEntity(DarkAnubis)]        = SpriteAlias{EntityID, olc::vi2d(10, 0)};
    m_aliases[aliasOfEntity(Cyclope)]           = SpriteAlias{EntityID, olc::vi2d(11, 0)};
    m_aliases[aliasOfEntity(Fool)]              = SpriteAlias{EntityID, olc::vi2d(12, 0)};
    m_aliases[aliasOfEntity(Beast)]             = SpriteAlias{EntityID, olc::vi2d(13, 0)};
    m_aliases[aliasOfEntity(BlueAvenger)]       = SpriteAlias{EntityID, olc::vi2d(14, 0)};
    m_aliases[aliasOfEntity(SoulEater)]         = SpriteAlias{EntityID, olc::vi2d(15, 0)};
    m_aliases[aliasOfEntity(Valkyrie)]          = SpriteAlias{EntityID, olc::vi2d(0, 1)};
    m_aliases[aliasOfEntity(Guardian)]          = SpriteAlias{EntityID, olc::vi2d(1, 1)};
    m_aliases[aliasOfEntity(Blob)]              = SpriteAlias{EntityID, olc::vi2d(2, 1)};
    m_aliases[aliasOfEntity(CosmicThreat)]      = SpriteAlias{EntityID, olc::vi2d(3, 1)};
    m_aliases[aliasOfEntity(Gorgone)]           = SpriteAlias{EntityID, olc::vi2d(4, 1)};
    m_aliases[aliasOfEntity(DemonBat)]          = SpriteAlias{EntityID, olc::vi2d(5, 1)};
    m_aliases[aliasOfEntity(Griffin)]           = SpriteAlias{EntityID, olc::vi2d(6, 1)};
    m_aliases[aliasOfEntity(Executioner)]       = SpriteAlias{EntityID, olc::vi2d(8, 1)};
    m_aliases[aliasOfEntity(MindlessGolem)]     = SpriteAlias{EntityID, olc::vi2d(9, 1)};
    m_aliases[aliasOfEntity(IncaOverlord)]      = SpriteAlias{EntityID, olc::vi2d(10, 1)};
    m_aliases[aliasOfEntity(Hydra)]             = SpriteAlias{EntityID, olc::vi2d(11, 1)};
    m_aliases[aliasOfEntity(Dragon)]            = SpriteAlias{EntityID, olc::vi2d(2, 2)};
    m_aliases[aliasOfEntity(Sorcerer)]          = SpriteAlias{EntityID, olc::vi2d(13, 2)};
    m_aliases[aliasOfEntity(Satyr)]             = SpriteAlias{EntityID, olc::vi2d(14, 2)};
    m_aliases[aliasOfEntity(TwoHeaderWarrior)]  = SpriteAlias{EntityID, olc::vi2d(15, 2)};
    m_aliases[aliasOfEntity(FireKnight)]        = SpriteAlias{EntityID, olc::vi2d(0, 3)};

    m_aliases[aliasOfEffect(Fire)]      = SpriteAlias{VFXID, olc::vi2d(0, 0)};
    m_aliases[aliasOfEffect(Lightning)] = SpriteAlias{VFXID, olc::vi2d(3, 0)};
    m_aliases[aliasOfEffect(Poison)]    = SpriteAlias{VFXID, olc::vi2d(6, 0)};
    m_aliases[aliasOfEffect(Smoke)]     = SpriteAlias{VFXID, olc::vi2d(9, 0)};

    m_aliases[spritesCount] = SpriteAlias{CursorID, olc::vi2d(0, 0)};
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

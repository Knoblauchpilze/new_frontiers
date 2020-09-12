
# include "NewFrontiersApp.hh"
# include "utils.hh"
# include "Controls.hh"

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
    m_menu(nullptr),
    m_aliases(),

    m_world(nullptr),
    m_wit(nullptr),

    m_cf(Viewport{olc::vf2d(-1.0f, 1.0f), olc::vf2d(10.0f, 15.0f)},
         Viewport{olc::vf2d(400.0f, 100.0f), olc::vf2d(width, height)},
         olc::vi2d(64, 32)),

    m_controls(newControls()),

    m_mLayer(0),
    m_dLayer(0),

    m_debugOn(true),
    m_state(State::Running),
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
    int spritesCount = tiles::BlocksCount + tiles::EntitiesCount + tiles::EffectsCount;
    m_aliases.resize(spritesCount + 1);

    m_aliases[aliasOfBlock(tiles::Empty)]              = SpriteAlias{SolidID, olc::vi2d(4, 2)};
    m_aliases[aliasOfBlock(tiles::Wall)]               = SpriteAlias{SolidID, olc::vi2d(0, 0)};
    m_aliases[aliasOfBlock(tiles::Door)]               = SpriteAlias{SolidID, olc::vi2d(0, 2)};

    m_aliases[aliasOfBlock(tiles::Portal)]             = SpriteAlias{PortalID, olc::vi2d(0, 0)};

    m_aliases[aliasOfEntity(tiles::MarineKnight)]      = SpriteAlias{EntityID, olc::vi2d(0, 0)};
    m_aliases[aliasOfEntity(tiles::Warrior)]           = SpriteAlias{EntityID, olc::vi2d(1, 0)};
    m_aliases[aliasOfEntity(tiles::StoneGiant)]        = SpriteAlias{EntityID, olc::vi2d(2, 0)};
    m_aliases[aliasOfEntity(tiles::Sorceress)]         = SpriteAlias{EntityID, olc::vi2d(3, 0)};
    m_aliases[aliasOfEntity(tiles::Knight)]            = SpriteAlias{EntityID, olc::vi2d(4, 0)};
    m_aliases[aliasOfEntity(tiles::GeneticExperiment)] = SpriteAlias{EntityID, olc::vi2d(5, 0)};
    m_aliases[aliasOfEntity(tiles::Warlord)]           = SpriteAlias{EntityID, olc::vi2d(6, 0)};
    m_aliases[aliasOfEntity(tiles::Vampire)]           = SpriteAlias{EntityID, olc::vi2d(7, 0)};
    m_aliases[aliasOfEntity(tiles::Gecko)]             = SpriteAlias{EntityID, olc::vi2d(9, 0)};
    m_aliases[aliasOfEntity(tiles::DarkAnubis)]        = SpriteAlias{EntityID, olc::vi2d(10, 0)};
    m_aliases[aliasOfEntity(tiles::Cyclope)]           = SpriteAlias{EntityID, olc::vi2d(11, 0)};
    m_aliases[aliasOfEntity(tiles::Fool)]              = SpriteAlias{EntityID, olc::vi2d(12, 0)};
    m_aliases[aliasOfEntity(tiles::Beast)]             = SpriteAlias{EntityID, olc::vi2d(13, 0)};
    m_aliases[aliasOfEntity(tiles::BlueAvenger)]       = SpriteAlias{EntityID, olc::vi2d(14, 0)};
    m_aliases[aliasOfEntity(tiles::SoulEater)]         = SpriteAlias{EntityID, olc::vi2d(15, 0)};
    m_aliases[aliasOfEntity(tiles::Valkyrie)]          = SpriteAlias{EntityID, olc::vi2d(0, 1)};
    m_aliases[aliasOfEntity(tiles::Guardian)]          = SpriteAlias{EntityID, olc::vi2d(1, 1)};
    m_aliases[aliasOfEntity(tiles::Blob)]              = SpriteAlias{EntityID, olc::vi2d(2, 1)};
    m_aliases[aliasOfEntity(tiles::CosmicThreat)]      = SpriteAlias{EntityID, olc::vi2d(3, 1)};
    m_aliases[aliasOfEntity(tiles::Gorgone)]           = SpriteAlias{EntityID, olc::vi2d(4, 1)};
    m_aliases[aliasOfEntity(tiles::DemonBat)]          = SpriteAlias{EntityID, olc::vi2d(5, 1)};
    m_aliases[aliasOfEntity(tiles::Griffin)]           = SpriteAlias{EntityID, olc::vi2d(6, 1)};
    m_aliases[aliasOfEntity(tiles::Executioner)]       = SpriteAlias{EntityID, olc::vi2d(8, 1)};
    m_aliases[aliasOfEntity(tiles::MindlessGolem)]     = SpriteAlias{EntityID, olc::vi2d(9, 1)};
    m_aliases[aliasOfEntity(tiles::IncaOverlord)]      = SpriteAlias{EntityID, olc::vi2d(10, 1)};
    m_aliases[aliasOfEntity(tiles::Hydra)]             = SpriteAlias{EntityID, olc::vi2d(11, 1)};
    m_aliases[aliasOfEntity(tiles::Dragon)]            = SpriteAlias{EntityID, olc::vi2d(2, 2)};
    m_aliases[aliasOfEntity(tiles::Sorcerer)]          = SpriteAlias{EntityID, olc::vi2d(13, 2)};
    m_aliases[aliasOfEntity(tiles::Satyr)]             = SpriteAlias{EntityID, olc::vi2d(14, 2)};
    m_aliases[aliasOfEntity(tiles::TwoHeadedWarrior)]  = SpriteAlias{EntityID, olc::vi2d(15, 2)};
    m_aliases[aliasOfEntity(tiles::FireKnight)]        = SpriteAlias{EntityID, olc::vi2d(0, 3)};

    m_aliases[aliasOfEffect(tiles::Fire)]      = SpriteAlias{VFXID, olc::vi2d(0, 0)};
    m_aliases[aliasOfEffect(tiles::Lightning)] = SpriteAlias{VFXID, olc::vi2d(3, 0)};
    m_aliases[aliasOfEffect(tiles::Poison)]    = SpriteAlias{VFXID, olc::vi2d(6, 0)};
    m_aliases[aliasOfEffect(tiles::Smoke)]     = SpriteAlias{VFXID, olc::vi2d(9, 0)};

    m_aliases[spritesCount] = SpriteAlias{CursorID, olc::vi2d(0, 0)};
  }

  void
  NewFrontiersApp::loadMenuResources() {
    // Create the menu: we want it to have a fixed
    // height and be at the bottom of the screen.
    int mHeight = 150;

    int w = ScreenWidth();
    int h = ScreenHeight();

    olc::vi2d mPos(0, h - mHeight);
    olc::vf2d mSize(w, mHeight);

    m_menu = std::make_shared<GameMenu>(mPos, mSize);
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

    olc::vi2d mPos = GetMousePos();
    m_controls.mPosX = mPos.x;
    m_controls.mPosY = mPos.y;

    // Detect clicks with the left mouse button to be
    // selecting game elements.
    if (GetMouse(0).bReleased) {
      log("Should detect element");
    }

    int scroll = GetMouseWheel();
    if (scroll > 0) {
      m_cf.zoom(Zoom::In, GetMousePos());
    }
    if (scroll < 0) {
      m_cf.zoom(Zoom::Out, GetMousePos());
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

    return true;
  }

  void
  NewFrontiersApp::draw() {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // Draw elements of the world.

    // Draw ground.
    for (int y = 0 ; y < m_wit->h() ; ++y) {
      for (int x = 0 ; x < m_wit->w() ; ++x) {
          drawSprite(x, y, tiles::Empty, 0);
      }
    }

    // Draw solid tiles.
    for (int id = 0 ; id < m_wit->blocksCount() ; ++id) {
      BlockDesc t = m_wit->block(id);
      drawSprite(t.tile.x, t.tile.y, aliasOfBlock(t.tile.type), t.tile.id);
      drawHealthBar(t.tile.x, t.tile.y, t.health, Cell::TopLeft);
    }

    // Draw entities.
    for (int id = 0 ; id < m_wit->entitiesCount() ; ++id) {
      EntityDesc t = m_wit->entity(id);

      if (t.state.glowing) {
        drawSprite(
          t.tile.x,
          t.tile.y,
          aliasOfEffect(tiles::Fire),
          2,
          Cell::CenterLeft,
          ALPHA_SEMI_OPAQUE
        );
      }
      if (t.state.exhausted) {
        drawSprite(
          t.tile.x,
          t.tile.y,
          aliasOfEffect(tiles::Poison),
          2,
          Cell::CenterLeft,
          ALPHA_SEMI_OPAQUE
        );
      }

      drawSprite(
        t.tile.x,
        t.tile.y,
        aliasOfEntity(t.tile.type),
        t.tile.id,
        Cell::CenterLeft
      );

      drawHealthBar(t.tile.x, t.tile.y, t.health, Cell::CenterLeft);
    }

    // Draw vfx.
    for (int id = 0 ; id < m_wit->vfxCount() ; ++id) {
      VFXDesc t = m_wit->vfx(id);
      drawSprite(
        t.tile.x,
        t.tile.y,
        aliasOfEffect(t.tile.type),
        t.tile.id,
        Cell::CenterLeft,
        static_cast<int>(std::round(ALPHA_OPAQUE * t.amount))
      );
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp);
    drawSprite(mtp.x, mtp.y, m_aliases.size() - 1, 0);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  NewFrontiersApp::drawUI() {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    if (m_menu != nullptr) {
      m_menu->render(this);
    }
    else {
      int mHeight = 150;
      int mBarHeight = 20;
      // int barToMenuGap = 15;

      int w = ScreenWidth();
      int h = ScreenHeight();

      olc::vf2d mPos(0.0f, h - mHeight);
      olc::vf2d mSize(w, mHeight);

      // Top bar of the menu (where quick access icons are).
      olc::Pixel c1(196, 162, 116);
      olc::Pixel c2(30, 17, 11);
      olc::Pixel c3(66, 48, 34);

      GradientFillRectDecal(
        mPos,
        olc::vf2d(mSize.x, mBarHeight * 0.66f),
        c1, c2, c2, c1
      );

      GradientFillRectDecal(
        mPos + olc::vf2d(0.0f, mBarHeight * 0.66f),
        olc::vf2d(mSize.x, mBarHeight * 0.34f),
        c2, c3, c3, c2
      );

      // Draw the minimap.
      // float mS = std::min(
      //   1.0f * m_mSprites.minimap->sprite->width,
      //   1.0f * mMainSize / m_mSprites.minimap->sprite->height
      // );

      // DrawDecal(
      //   mPos + olc::vf2d(0.0f, mBarHeight + barToMenuGap),
      //   m_mSprites.minimap,
      //   olc::vf2d(mS, mS)
      // );
      // Load the minimap image.
      // spr = new olc::Sprite("data/img/minimap.png");
      // m_mSprites.minimap = new olc::Decal(spr);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  NewFrontiersApp::drawDebug() {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // If the debug mode is deactivated, return
    // now as we don't want to display anything.
    if (!m_debugOn) {
      return;
    }

    // Render entities path and position
    for (int id = 0 ; id < m_wit->entitiesCount() ; ++id) {
      EntityDesc ed = m_wit->entity(id);

      olc::vf2d o = m_cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, Cell::Center);
      olc::vf2d t = m_cf.tileCoordsToPixels(ed.xT, ed.yT, Cell::Center);
      olc::vf2d tl = m_cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, Cell::CenterLeft);
      olc::vf2d bc = m_cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, Cell::Center);

      for (unsigned id = 0u ; id < ed.cPoints.size() / 2u ; ++id) {
        olc::vf2d p = m_cf.tileCoordsToPixels(ed.cPoints[2 * id], ed.cPoints[2 * id + 1], Cell::Center);
        FillCircle(p, 3, olc::CYAN);
      }

      DrawLine(o, t, olc::WHITE);
      DrawRect(tl, m_ss * m_cf.tileScale(), olc::MAGENTA);
      FillCircle(bc, 5, olc::YELLOW);
    }

    // Render mouse and world cell coordinates.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = m_cf.pixelCoordsToTiles(mp, &it);

    int dOffset = 15;
    DrawString(olc::vi2d(0, 0 * dOffset), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    // Not the first frame anymore.
    m_first = false;

    SetPixelMode(olc::Pixel::NORMAL);
  }

}

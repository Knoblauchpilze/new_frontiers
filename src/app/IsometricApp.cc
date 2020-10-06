
# include "IsometricApp.hh"
# include "utils.hh"
# include "coordinates/IsometricFrame.hh"

namespace new_frontiers {

  IsometricApp::IsometricApp(const olc::vi2d& dims,
                             const Theme& theme,
                             const std::string& name):
    PGEApp(
      newDesc(
        dims,
        std::make_shared<IsometricFrame>(
          Viewport{olc::vf2d(1.0f, 1.0f), olc::vf2d(10.0f, 15.0f)},
          Viewport{olc::vf2d(300.0f, 50.0f), olc::vf2d(640.0f, 480.0f)},
          olc::vi2d(64, 32)
        ),
        name
      )
    ),

    m_ss(theme.size),
    m_sprites(),
    m_aliases()
  {
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

  void
  IsometricApp::loadResources() {
    // Load the sprites containing textures as
    // defined by the theme.
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
  IsometricApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // Draw elements of the world.
    SpriteDesc sd;

    // Draw ground.
    sd.type = tiles::Empty;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int y = 0 ; y < res.wit->h() ; ++y) {
      for (int x = 0 ; x < res.wit->w() ; ++x) {
        sd.x = x;
        sd.y = y;

        drawSprite(sd, res.cf, 0);
      }
    }

    // Draw solid tiles.
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int id = 0 ; id < res.wit->blocksCount() ; ++id) {
      BlockDesc t = res.wit->block(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;
      sd.type = aliasOfBlock(t.tile.type);

      drawSprite(sd, res.cf, t.tile.id);
      drawHealthBar(sd, t.health, res.cf);
    }

    // Draw entities.
    sd.location = Cell::UpperLeft;

    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc t = res.wit->entity(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;
      sd.radius = t.radius;

      if (t.state.glowing) {
        sd.type = aliasOfEffect(tiles::Fire);
        sd.alpha = ALPHA_SEMI_OPAQUE;

        drawSprite(sd, res.cf, 2);
      }
      if (t.state.exhausted) {
        sd.type = aliasOfEffect(tiles::Poison);
        sd.alpha = ALPHA_SEMI_OPAQUE;

        drawSprite(sd, res.cf, 2);
      }

      sd.type = aliasOfEntity(t.tile.type);
      sd.alpha = ALPHA_OPAQUE;

      drawSprite(sd, res.cf, t.tile.id);
      drawHealthBar(sd, t.health, res.cf);

      if (t.cargo > 0.0f) {
        sd.location = Cell::BottomLeft;
        float r = t.carrying / t.cargo;
        drawHealthBar(sd, r, res.cf, Orientation::Vertical);
        sd.location = Cell::UpperLeft;
      }
    }

    // Draw vfx.
    sd.type = VFXID;
    sd.location = Cell::UpperLeft;

    for (int id = 0 ; id < res.wit->vfxCount() ; ++id) {
      VFXDesc t = res.wit->vfx(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;
      sd.type = aliasOfEffect(t.tile.type);
      sd.alpha = static_cast<int>(std::round(ALPHA_OPAQUE * t.amount));
      sd.radius = t.radius;

      drawSprite(sd, res.cf, t.tile.id);
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp);

    sd.x = mtp.x;
    sd.y = mtp.y;
    sd.type = m_aliases.size() - 1;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    drawSprite(sd, res.cf, 0);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  IsometricApp::drawUI(const RenderDesc& res) {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    if (res.ui != nullptr) {
      res.ui->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  IsometricApp::drawDebug(const RenderDesc& res) {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Render entities path and position
    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc ed = res.wit->entity(id);

      olc::vf2d cb = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius);
      olc::vf2d tcb = res.cf.tileCoordsToPixels(ed.xT, ed.yT, ed.radius);
      olc::vf2d ul = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius, Cell::UpperLeft);

      for (unsigned id = 0u ; id < ed.cPoints.size() / 2u ; ++id) {
        olc::vf2d p = res.cf.tileCoordsToPixels(ed.cPoints[2 * id], ed.cPoints[2 * id + 1], 1.0f);
        FillCircle(p, 3, olc::CYAN);
      }

      DrawLine(cb, tcb, olc::WHITE);
      DrawRect(ul, ed.radius * res.cf.tileScale() * m_ss, olc::MAGENTA);
      FillCircle(cb, 5, olc::YELLOW);

      // TODO: In case the size of the tile is different in
      // one direction and the other it means that we should
      // in fact draw an ellipse.
      olc::vf2d tlpr = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.percepRadius);
      DrawCircle(tlpr, ed.percepRadius * res.cf.tileSize().x, olc::BLUE);
    }

    // Render mouse and world cell coordinates.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int dOffset = 15;
    DrawString(olc::vi2d(0, 0 * dOffset), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    SetPixelMode(olc::Pixel::NORMAL);
  }

}

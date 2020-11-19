
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
          Viewport{olc::vf2d(1.0f, 1.0f), olc::vf2d(10.0f, 10.0f)},
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

    // Draw elements of the world. We will first
    // fetch the visible elements and then paint
    // them.
    Viewport v = res.cf.cellsViewport();
    std::vector<world::ItemEntry> items = res.loc->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      nullptr,
      nullptr,
      world::Sort::ZOrder
    );

    log(
      "Fetched " + std::to_string(items.size()) + " element(s)" +
      " in viewport [" + std::to_string(v.p.x) + "," + std::to_string(v.p.y) +
      " : " + std::to_string(v.dims.x) + "x" + std::to_string(v.dims.y) + "]",
      utils::Level::Verbose
    );

    SpriteDesc sd;

    // No matter what happens we can draw the ground
    // so that we always see something on screen.
    sd.type = tiles::Empty;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int y = 0 ; y < res.loc->h() ; ++y) {
      for (int x = 0 ; x < res.loc->w() ; ++x) {
        sd.x = x;
        sd.y = y;

        drawSprite(sd, res.cf, 0);
      }
    }

    // Then iterate over the visible items and draw
    // them in order (as we requested the entries to
    // be sorted in ascending `z` order).
    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const world::ItemEntry& ie = items[id];

      // Case of a block.
      if (ie.type == world::ItemType::Block) {
        world::Block t = res.loc->block(ie.index);

        sd.alpha = ALPHA_OPAQUE;
        sd.radius = 1.0f;
        sd.location = Cell::TopLeft;
        sd.x = t.tile.p.x;
        sd.y = t.tile.p.y;
        sd.type = aliasOfBlock(t.tile.type);

        drawSprite(sd, res.cf, t.tile.id);

        // Draw the healthbar only in case of `interesting`
        // buildings like anything apart from walls.
        if (t.tile.type != tiles::Wall) {
          drawHealthBar(sd, t.health, res.cf);
        }

        if (t.ratio > 0.0f) {
          sd.location = Cell::BottomLeft;
          drawHealthBar(sd, t.ratio, res.cf, Orientation::Vertical);
        }
      }

      // Case of an entity.
      if (ie.type == world::ItemType::Entity) {
        world::Entity t = res.loc->entity(ie.index);

        sd.radius = t.radius;
        sd.location = Cell::UpperLeft;
        sd.x = t.tile.p.x;
        sd.y = t.tile.p.y;

        if (t.state.glowing) {
          sd.alpha = ALPHA_SEMI_OPAQUE;
          sd.type = aliasOfEffect(tiles::Fire);

          drawSprite(sd, res.cf, 2);
        }
        if (t.state.exhausted) {
          sd.alpha = ALPHA_SEMI_OPAQUE;
          sd.type = aliasOfEffect(tiles::Poison);

          drawSprite(sd, res.cf, 2);
        }

        sd.alpha = ALPHA_OPAQUE;
        sd.type = aliasOfEntity(t.tile.type);

        drawSprite(sd, res.cf, t.tile.id);
        drawHealthBar(sd, t.health, res.cf);

        if (t.cargo > 0.0f) {
          sd.location = Cell::BottomLeft;
          float r = t.carrying / t.cargo;
          drawHealthBar(sd, r, res.cf, Orientation::Vertical);
        }
      }

      // Case of a VFX.
      if (ie.type == world::ItemType::VFX) {
        world::VFX t = res.loc->vfx(ie.index);

        sd.alpha = static_cast<int>(std::round(ALPHA_OPAQUE * t.amount));
        sd.radius = t.radius;
        sd.location = Cell::UpperLeft;
        sd.x = t.tile.p.x;
        sd.y = t.tile.p.y;
        sd.type = aliasOfEffect(t.tile.type);

        drawSprite(sd, res.cf, t.tile.id);
      }
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

    // Draw the colonies. We first need to draw the
    // transparent background and then draw each
    // colony's information.
    int cOffset = 3;
    int icOffset = 2;
    olc::vi2d s(0.0f, cOffset);
    olc::Pixel bg(255, 255, 255, ALPHA_SEMI_OPAQUE);
    int tpSize = 20;

    for (int id = 0 ; id < res.loc->coloniesCount() ; ++id) {
      world::Colony c = res.loc->colony(id);
      olc::vi2d idS = GetTextSize(c.id.toString());

      olc::vi2d idFocus = GetTextSize(world::focusToString(c.focus));

      s.x = std::max(s.x, icOffset + idS.x + icOffset + tpSize + icOffset + idFocus.x + icOffset);
      s.y += (idS.y + cOffset);
    }

    // Draw the general background as semi opaque.
    FillRectDecal(olc::vf2d(), s, bg);

    olc::vf2d p(icOffset, cOffset);
    for (int id = 0 ; id < res.loc->coloniesCount() ; ++id) {
      world::Colony c = res.loc->colony(id);

      olc::vi2d idS = GetTextSize(c.id.toString());

      // Override the background with a different
      // opacity based on whether the colony is
      // active.
      if (c.active) {
        olc::Pixel uc = (c.active ? olc::Pixel(bg.r, bg.g, bg.b, ALPHA_OPAQUE) : bg);
        FillRectDecal(olc::vf2d(0.0f, p.y - cOffset / 2.0f), olc::vf2d(s.x, idS.y + cOffset), uc);
      }

      // Draw the colony's identifier.
      DrawStringDecal(p, c.id.toString(), (c.active ? olc::BLACK : olc::WHITE));

      // Draw the bar corresponding to the resource
      // budget.
      olc::vf2d tp(icOffset + idS.x + icOffset, p.y);
      olc::Pixel tint = redToGreenGradient(c.ratio, ALPHA_OPAQUE);
      olc::Pixel emptyTint(
        static_cast<int>(tint.r * 0.5f),
        static_cast<int>(tint.g * 0.5f),
        static_cast<int>(tint.b * 0.5f),
        tint.a
      );

      FillRectDecal(tp, olc::vf2d(c.ratio * tpSize, idS.y), tint);
      FillRectDecal(
        olc::vf2d(tp.x + c.ratio * tpSize, tp.y),
        olc::vf2d((1.0f - c.ratio) * tpSize, idS.y),
        emptyTint
      );

      // Draw the current focus of this colony rendered
      // as a string.
      olc::vf2d tf(icOffset + idS.x + tpSize + icOffset, p.y);
      olc::Pixel fTint = olc::WHITE;
      switch (c.focus) {
        case colony::Priority::Consolidation:
          fTint = olc::GREEN;
          break;
        case colony::Priority::Expansion:
          fTint = olc::BLUE;
          break;
        case colony::Priority::War:
          fTint = olc::RED;
          break;
        default:
          break;
      }

      DrawStringDecal(tf, world::focusToString(c.focus), fTint);

      p.y += (idS.y + cOffset);
    }

    // Render the game menu.
    if (res.ui != nullptr) {
      res.ui->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  IsometricApp::drawDebug(const RenderDesc& res) {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Render entities path and position.
    Viewport v = res.cf.cellsViewport();
    world::ItemType ie = world::ItemType::Entity;
    std::vector<world::ItemEntry> items = res.loc->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      &ie,
      nullptr,
      world::Sort::ZOrder
    );

    for (unsigned i = 0 ; i < items.size() ; ++i) {
      const world::ItemEntry& ie = items[i];

      // Be on the safe side.
      if (ie.type != world::ItemType::Entity) {
        log(
          "Fetched item with type " + std::to_string(static_cast<int>(ie.type)) + " while requesting only entities",
          utils::Level::Warning
        );

        continue;
      }

      world::Entity ed = res.loc->entity(ie.index);

      olc::vf2d cb = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.radius);

      // Draw the path of this entity if any.
      if (ed.path.valid()) {
        olc::vf2d old;
        for (unsigned id = 0u ; id < ed.path.cPoints.size() ; ++id) {
          olc::vf2d p = res.cf.tileCoordsToPixels(ed.path.cPoints[id].x, ed.path.cPoints[id].y, 1.0f);
          FillCircle(p, 3, olc::CYAN);

          if (id > 0u) {
            DrawLine(old, p, olc::WHITE);
          }
          old = p;
        }
      }

      // Draw the rectangle around the entity and its circle
      // to represent its position.
      olc::vf2d ul = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.radius, Cell::UpperLeft);
      DrawRect(ul, ed.radius * res.cf.tileScale() * m_ss, olc::MAGENTA);
      FillCircle(cb, 5, olc::YELLOW);

      // TODO: In case the size of the tile is different in
      // one direction and the other it means that we should
      // in fact draw an ellipse.
      olc::vf2d tlpr = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.percepRadius);
      DrawCircle(tlpr, ed.percepRadius * res.cf.tileSize().y, olc::BLUE);
    }

    // Render mouse and world cell coordinates.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 3 * dOffset), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 2 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 1 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    SetPixelMode(olc::Pixel::NORMAL);
  }

}

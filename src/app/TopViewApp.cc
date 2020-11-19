
# include "TopViewApp.hh"
# include "utils.hh"
# include "coordinates/TopViewFrame.hh"

namespace new_frontiers {

  TopViewApp::TopViewApp(const olc::vi2d& dims,
                         const std::string& name):
    PGEApp(
      newDesc(
        dims,
        std::make_shared<TopViewFrame>(
          Viewport{olc::vf2d(1.0f, 1.0f), olc::vf2d(10.0f, 15.0f)},
          Viewport{olc::vf2d(150.0f, 50.0f), olc::vf2d(640.0f, 480.0f)},
          olc::vi2d(16, 32)
        ),
        name
      )
    ),

    m_sprites()
  {}

  void
  TopViewApp::loadResources() {
    // Load the colors used to represent elements.
    m_sprites.resize(SpriteTypesCount);

    m_sprites[GroundID] = olc::VERY_DARK_GREY;
    m_sprites[SolidID] = olc::DARK_GREY;
    m_sprites[PortalID] = olc::Pixel(255, 128, 0);
    m_sprites[EntityID] = olc::VERY_DARK_CYAN;
    m_sprites[VFXID] = olc::VERY_DARK_RED;
    m_sprites[CursorID] = olc::YELLOW;
  }

  void
  TopViewApp::draw(const RenderDesc& res) {
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
    sd.type = GroundID;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int y = 0 ; y < res.loc->h() ; ++y) {
      for (int x = 0 ; x < res.loc->w() ; ++x) {
        sd.x = x;
        sd.y = y;

        drawSprite(sd, res.cf);
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

        switch (t.tile.type) {
          case tiles::Portal:
            sd.type = PortalID;
            break;
          case tiles::Wall:
          case tiles::Door:
          default:
            sd.type = SolidID;
            break;
        }

        drawSprite(sd, res.cf);
        drawHealthBar(sd, t.health, res.cf);

        if (t.ratio > 0.0f) {
          sd.location = Cell::BottomLeft;
          drawHealthBar(sd, t.ratio, res.cf, Orientation::Vertical);
        }
      }

      // Case of an entity.
      if (ie.type == world::ItemType::Entity) {
        world::Entity t = res.loc->entity(ie.index);

        sd.radius = t.radius;
        sd.location = Cell::UpperTopLeft;
        sd.x = t.tile.p.x;
        sd.y = t.tile.p.y;

        if (t.state.glowing) {
          sd.alpha = ALPHA_SEMI_OPAQUE;
          sd.type = VFXID;

          drawSprite(sd, res.cf);
        }
        if (t.state.exhausted) {
          sd.alpha = ALPHA_SEMI_OPAQUE;
          sd.type = VFXID;

          drawSprite(sd, res.cf);
        }

        sd.alpha = ALPHA_OPAQUE;
        sd.type = EntityID;

        drawSprite(sd, res.cf);
        drawHealthBar(sd, t.health, res.cf);

        if (t.cargo > 0.0f) {
          sd.location = Cell::UpperEast;
          float r = t.carrying / t.cargo;
          drawHealthBar(sd, r, res.cf, Orientation::Vertical);
        }
      }

      // Case of a VFX.
      if (ie.type == world::ItemType::VFX) {
        world::VFX t = res.loc->vfx(ie.index);

        sd.alpha = static_cast<int>(std::round(ALPHA_OPAQUE * t.amount));
        sd.radius = t.radius;
        sd.location = Cell::UpperTopLeft;
        sd.x = t.tile.p.x;
        sd.y = t.tile.p.y;
        sd.type = VFXID;

        drawSprite(sd, res.cf);
      }
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp);

    sd.x = mtp.x;
    sd.y = mtp.y;
    sd.type = CursorID;
    sd.alpha = ALPHA_SEMI_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    drawSprite(sd, res.cf);

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  TopViewApp::drawUI(const RenderDesc& res) {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Draw the colonies. We first need to draw the
    // transparent background and then draw each
    // colony's information.
    olc::vi2d s;
    int cOffset = 3;
    int icOffset = 2;
    olc::Pixel bg(255, 255, 255, ALPHA_SEMI_OPAQUE);
    int tpSize = 20;

    for (int id = 0 ; id < res.loc->coloniesCount() ; ++id) {
      world::Colony c = res.loc->colony(id);
      olc::vi2d idS = GetTextSize(c.id.toString());

      olc::vi2d idFocus = GetTextSize(world::focusToString(c.focus));

      s.x = std::max(s.x, idS.x + icOffset + tpSize + icOffset + idFocus.x);
      s.y += (idS.y + cOffset);
    }

    FillRectDecal(olc::vf2d(), s, bg);

    olc::vf2d p;
    for (int id = 0 ; id < res.loc->coloniesCount() ; ++id) {
      world::Colony c = res.loc->colony(id);

      // Draw the colony's identifier.
      DrawStringDecal(p, c.id.toString());

      olc::vi2d idS = GetTextSize(c.id.toString());

      // Draw the bar corresponding to the resource
      // budget.
      olc::vf2d tp(idS.x + icOffset, p.y);
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
      olc::vf2d tf(idS.x + tpSize + icOffset, p.y);
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
  TopViewApp::drawDebug(const RenderDesc& res) {
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

      olc::vf2d tl = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.radius, Cell::TopLeft);

      // Draw the path of this entity if any.
      if (ed.path.valid()) {
        olc::vf2d old;
        for (unsigned id = 0u ; id < ed.path.cPoints.size() ; ++id) {
          olc::vf2d p = res.cf.tileCoordsToPixels(ed.path.cPoints[id].x, ed.path.cPoints[id].y, 1.0f, Cell::TopLeft);
          FillCircle(p, 3, olc::CYAN);

          if (id > 0u) {
            DrawLine(old, p, olc::WHITE);
          }
          old = p;
        }
      }

      // Draw the rectangle around the entity and its circle
      // to represent its position.
      olc::vf2d cb = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.radius, Cell::UpperTopLeft);
      DrawRect(cb, ed.radius * res.cf.tileSize(), olc::MAGENTA);
      FillCircle(tl, 5, olc::YELLOW);

      // TODO: In case the size of the tile is different in
      // one direction and the other it means that we should
      // in fact draw an ellipse.
      olc::vf2d tlpr = res.cf.tileCoordsToPixels(ed.tile.p.x, ed.tile.p.y, ed.percepRadius, Cell::TopLeft);
      DrawCircle(tlpr, ed.percepRadius * res.cf.tileSize().x, olc::BLUE);
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

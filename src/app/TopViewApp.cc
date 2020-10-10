
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
      true
    );

    log(
      "Fetched " + std::to_string(items.size()) + " element(s)" +
      " in viewport [" + std::to_string(v.p.x) + "," + std::to_string(v.p.y) +
      " : " + std::to_string(v.dims.x) + "x" + std::to_string(v.dims.y) + "]"
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
        sd.x = t.tile.x;
        sd.y = t.tile.y;

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
        sd.x = t.tile.x;
        sd.y = t.tile.y;

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
        sd.x = t.tile.x;
        sd.y = t.tile.y;
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
      true
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

      olc::vf2d cb = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius, Cell::UpperTopLeft);
      olc::vf2d ttl = res.cf.tileCoordsToPixels(ed.xT, ed.yT, ed.radius, Cell::TopLeft);
      olc::vf2d tl = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius, Cell::TopLeft);

      for (unsigned id = 0u ; id < ed.cPoints.size() / 2u ; ++id) {
        olc::vf2d p = res.cf.tileCoordsToPixels(ed.cPoints[2 * id], ed.cPoints[2 * id + 1], 1.0f, Cell::TopLeft);
        FillCircle(p, 3, olc::CYAN);
      }

      DrawLine(tl, ttl, olc::WHITE);
      DrawRect(cb, ed.radius * res.cf.tileSize(), olc::MAGENTA);
      FillCircle(tl, 5, olc::YELLOW);

      // TODO: In case the size of the tile is different in
      // one direction and the other it means that we should
      // in fact draw an ellipse.
      olc::vf2d tlpr = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.percepRadius, Cell::TopLeft);
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

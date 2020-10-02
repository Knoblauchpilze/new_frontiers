
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
    m_sprites[PortalID] = olc::GREY;
    m_sprites[EntityID] = olc::VERY_DARK_CYAN;
    m_sprites[VFXID] = olc::VERY_DARK_RED;
    m_sprites[CursorID] = olc::YELLOW;
  }

  void
  TopViewApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // Draw elements of the world.
    SpriteDesc sd;

    // Draw ground.
    sd.type = GroundID;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int y = 0 ; y < res.wit->h() ; ++y) {
      for (int x = 0 ; x < res.wit->w() ; ++x) {
        sd.x = x;
        sd.y = y;

        drawSprite(sd, res.cf);
      }
    }

    // Draw solid tiles.
    sd.type = SolidID;
    sd.alpha = ALPHA_OPAQUE;
    sd.radius = 1.0f;
    sd.location = Cell::TopLeft;

    for (int id = 0 ; id < res.wit->blocksCount() ; ++id) {
      BlockDesc t = res.wit->block(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;

      drawSprite(sd, res.cf);
      drawHealthBar(sd, t.health, res.cf);
    }

    // Draw entities.
    sd.location = Cell::CenterBottom;

    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc t = res.wit->entity(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;
      sd.radius = t.radius;

      if (t.state.glowing) {
        sd.type = VFXID;
        sd.alpha = ALPHA_SEMI_OPAQUE;

        drawSprite(sd, res.cf);
      }
      if (t.state.exhausted) {
        sd.type = VFXID;
        sd.alpha = ALPHA_SEMI_OPAQUE;

        drawSprite(sd, res.cf);
      }

      sd.type = EntityID;
      sd.alpha = ALPHA_OPAQUE;

      drawSprite(sd, res.cf);
      drawHealthBar(sd, t.health, res.cf);
    }

    // Draw vfx.
    sd.type = VFXID;
    sd.location = Cell::CenterBottom;

    for (int id = 0 ; id < res.wit->vfxCount() ; ++id) {
      VFXDesc t = res.wit->vfx(id);

      sd.x = t.tile.x;
      sd.y = t.tile.y;
      sd.alpha = static_cast<int>(std::round(ALPHA_OPAQUE * t.amount));
      sd.radius = t.radius;

      drawSprite(sd, res.cf);
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


    // Render entities path and position
    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc ed = res.wit->entity(id);

      olc::vf2d cb = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius);
      olc::vf2d ttl = res.cf.tileCoordsToPixels(ed.xT, ed.yT, ed.radius, Cell::TopLeft);
      olc::vf2d tl = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, ed.radius, Cell::TopLeft);

      for (unsigned id = 0u ; id < ed.cPoints.size() / 2u ; ++id) {
        olc::vf2d p = res.cf.tileCoordsToPixels(ed.cPoints[2 * id], ed.cPoints[2 * id + 1], 1.0f, Cell::TopLeft);
        FillCircle(p, 3, olc::CYAN);
      }

      DrawLine(tl, ttl, olc::WHITE);
      DrawRect(cb, ed.radius * res.cf.tileSize(), olc::MAGENTA);
      FillCircle(tl, 5, olc::YELLOW);
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

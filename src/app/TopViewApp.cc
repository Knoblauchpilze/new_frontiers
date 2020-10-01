
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
          Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(10.0f, 15.0f)},
          Viewport{olc::vf2d(100.0f, 50.0f), olc::vf2d(640.0f, 480.0f)},
          olc::vi2d(64, 32)
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

    // Draw ground.
    for (int y = 0 ; y < res.wit->h() ; ++y) {
      for (int x = 0 ; x < res.wit->w() ; ++x) {
        drawSprite(res.cf.tileCoordsToPixels(x, y), res.cf.tileSize(), GroundID, ALPHA_OPAQUE - std::rand() % 50);
      }
    }

    // Draw solid tiles.
    for (int id = 0 ; id < res.wit->blocksCount() ; ++id) {
      BlockDesc t = res.wit->block(id);
      drawSprite(res.cf.tileCoordsToPixels(t.tile.x, t.tile.y), res.cf.tileSize(), SolidID);
      drawHealthBar(res.cf.tileCoordsToPixels(t.tile.x, t.tile.y), res.cf.tileSize(), t.health);
    }

    // Draw entities.
    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc t = res.wit->entity(id);

      if (t.state.glowing) {
        drawSprite(
          res.cf.tileCoordsToPixels(t.tile.x, t.tile.y, Cell::TopLeft),
          res.cf.tileSize(),
          VFXID,
          ALPHA_SEMI_OPAQUE
        );
      }
      if (t.state.exhausted) {
        drawSprite(
          res.cf.tileCoordsToPixels(t.tile.x, t.tile.y, Cell::TopLeft),
          res.cf.tileSize(),
          VFXID,
          ALPHA_SEMI_OPAQUE
        );
      }

      drawSprite(
        res.cf.tileCoordsToPixels(t.tile.x, t.tile.y, Cell::TopLeft),
        res.cf.tileSize(),
        EntityID
      );

      drawHealthBar(
        res.cf.tileCoordsToPixels(t.tile.x, t.tile.y, Cell::TopLeft),
        res.cf.tileSize(),
        t.health
      );
    }

    // Draw vfx.
    for (int id = 0 ; id < res.wit->vfxCount() ; ++id) {
      VFXDesc t = res.wit->vfx(id);
      drawSprite(
        res.cf.tileCoordsToPixels(t.tile.x, t.tile.y, Cell::TopLeft),
        res.cf.tileSize(),
        VFXID,
        static_cast<int>(std::round(ALPHA_OPAQUE * t.amount))
      );
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp);
    drawSprite(res.cf.tileCoordsToPixels(mtp.x, mtp.y), res.cf.tileSize(), CursorID, ALPHA_SEMI_OPAQUE);

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

    // If the debug mode is deactivated, return
    // now as we don't want to display anything.
    if (!hasDebug()) {
      return;
    }

    // Render entities path and position
    for (int id = 0 ; id < res.wit->entitiesCount() ; ++id) {
      EntityDesc ed = res.wit->entity(id);

      olc::vf2d t = res.cf.tileCoordsToPixels(ed.xT, ed.yT);
      olc::vf2d tl = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y, Cell::TopLeft);
      olc::vf2d bc = res.cf.tileCoordsToPixels(ed.tile.x, ed.tile.y);

      for (unsigned id = 0u ; id < ed.cPoints.size() / 2u ; ++id) {
        olc::vf2d p = res.cf.tileCoordsToPixels(ed.cPoints[2 * id], ed.cPoints[2 * id + 1]);
        FillCircle(p, 3, olc::CYAN);
      }

      DrawLine(bc, t, olc::WHITE);
      DrawRect(tl, res.cf.tileSize(), olc::MAGENTA);
      FillCircle(bc, 5, olc::YELLOW);
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

#ifndef    NEW_FRONTIERS_APP_HXX
# define   NEW_FRONTIERS_APP_HXX

# include "NewFrontiersApp.hh"

namespace new_frontiers {

  inline
  NewFrontiersApp::~NewFrontiersApp() {
    if (m_sprite != nullptr) {
      delete m_sprite;
    }
  }

  inline
  bool
  NewFrontiersApp::OnUserCreate() {
    // Create the world.
    m_world = std::make_shared<World>(10, 8, 6);

    // And the tile alias.
    createTileAliases();

    return true;
  }

  inline
  olc::vi2d
  NewFrontiersApp::spriteSize() noexcept {
    return olc::vi2d(64, 64);
  }

  inline
  olc::vi2d
  NewFrontiersApp::spriteCoordsToPixels(int x, int y) const noexcept {
    return olc::vi2d(x * 64, y * 64);
  }

  inline
  olc::vi2d
  NewFrontiersApp::tileSize() noexcept {
    return olc::vi2d(64, 32);
  }

  inline
  olc::vi2d
  NewFrontiersApp::tileCoordsToPixels(int x, int y) const noexcept {
    return olc::vi2d(
      m_wox * 64 + (y - x) * 64 / 2,
      m_woy * 32 + (x + y) * 32 / 2 - 32
    );
  }

  inline
  olc::vi2d
  NewFrontiersApp::pixelCoordsToTiles(const olc::vi2d& pixels) const noexcept {
    int tx = pixels.x / 64 - m_wox;
    int ty = pixels.y / 32 - m_woy;

    olc::vi2d rt(ty - tx, ty + tx);

    // We need to adjust for the following situation:
    //
    // +--+
    // |i/
    // |/
    // +
    //
    // Basically we need to:
    // - decrease the `y` coordinate by `1` in the
    //   top left corner.
    // - decrease the `x` coordinate by `1` in the
    //   top right corner.
    // - increase the `x` coordinate by `1` in the
    //   bottom left corner.
    // - increase the `y` coordinate by `1` in the
    //   bottom right corner.
    //
    // To detect such cases, we know that the coords
    // of the mouse within the cell should be such
    // that:
    // - `y < h/2 - xh/w` for the TLC.
    // - `y < xh/w - h/2` for the TRC.
    // - `y > h/2 + xh/w` for the BLC.
    // - `y > 3h/2 - xh/w` for the BRC.
    //
    // Compute the offset of the input position in
    // the tile itself.
    int x = pixels.x % 64;
    int y = pixels.y % 32;

    float hw = 64.0f / 2.0f;
    float hh = 32.0f / 2.0f;
    float how = 32.0f / 64.0f;

    // Now detect each corner and adjust the coordinate
    // of the cell.
    if (x < hw && y < hh && y < hh - x * how) {
      // Top left corner.
      --rt.y;
    }
    if (x > hw && y < hh && y < x * how - hh) {
      // Top right corner.
      --rt.x;
    }
    if (x < hw && y > hh && y > hh + x * how) {
      // Bottom left corner.
      ++rt.x;
    }
    if (x > hw && y > hh && y > 3 * hh - x * how) {
      // Bottom right corner.
      ++rt.y;
    }

    return rt;
  }

  inline
  void
  NewFrontiersApp::initialize(int width, int height, int pixelRatio) {
    // Construct the window. Note that we use a pixel size
    // to screen size ratio of `1` (meaning that each pixel
    // of the viewport will be represented by a pixel on
    // the screen).
    olc::rcode c = Construct(width, height, pixelRatio, pixelRatio);

    if (c != olc::OK) {
      throw utils::CoreException(
        std::string("Could not build new frontiers application"),
        std::string("Initialization failed")
      );
    }
  }

  inline
  int
  NewFrontiersApp::aliasOfSprite(const Sprite& sprite) {
    return sprite;
  }

  inline
  int
  NewFrontiersApp::aliasOfEntity(const Mob& ent) {
    return SpritesCount + ent;
  }

  inline
  int
  NewFrontiersApp::aliasOfEffect(const Effect& vfx) {
    return SpritesCount + MobsCount + vfx;
  }

}

#endif    /* NEW_FRONTIERS_APP_HXX */

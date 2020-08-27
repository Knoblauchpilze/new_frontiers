#ifndef    NEW_FRONTIERS_APP_HXX
# define   NEW_FRONTIERS_APP_HXX

# include "NewFrontiersApp.hh"

namespace new_frontiers {

  inline
  olc::vi2d
  NewFrontiersApp::spriteSize() noexcept {
    return olc::vi2d(64, 64);
  }

  inline
  olc::vi2d
  NewFrontiersApp::spriteCoordsToPixels(int x, int y) noexcept {
    return olc::vi2d(x * 64, y * 64);
  }

  inline
  olc::vi2d
  NewFrontiersApp::tileSize() noexcept {
    return olc::vi2d(64, 32);
  }

  inline
  olc::vi2d
  NewFrontiersApp::tileCoordsToPixels(const olc::vi2d& tiles, bool full) noexcept {
    olc::vi2d pc(
      WORLD_ORIGIN_X * 64 + (tiles.y - tiles.x) * 64 / 2,
      WORLD_ORIGIN_Y * 32 + (tiles.x + tiles.y) * 32 / 2
    );

    pc.y -= (full ? 64 : 32);

    return pc;
  }

  inline
  olc::vi2d
  NewFrontiersApp::pixelCoordsToTiles(const olc::vi2d& pixels) noexcept {
    int tx = pixels.x / 64 - WORLD_ORIGIN_X;
    int ty = pixels.y / 32 - WORLD_ORIGIN_Y;

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
  olc::vi2d
  NewFrontiersApp::RNGUtils::coords() {
    return olc::vi2d(wRNG(rng), hRNG(rng));
  }

  inline
  void
  NewFrontiersApp::RNGUtils::reset(unsigned w,
                                   unsigned h)
  {
    wRNG.param(std::uniform_int_distribution<int>::param_type(1, w));
    hRNG.param(std::uniform_int_distribution<int>::param_type(1, h));
  }

}

#endif    /* NEW_FRONTIERS_APP_HXX */

#ifndef    COORDINATE_FRAMES_HXX
# define   COORDINATE_FRAMES_HXX

# include "CoordinateFrames.hh"

namespace new_frontiers {

  inline
  CoordinateFrames::CoordinateFrames(const olc::vi2d& worldOrigin,
                                     const olc::vi2d& sprite,
                                     const olc::vi2d& tile):
    m_wo(worldOrigin),

    m_ss(sprite),
    m_ts(tile)
  {}


  inline
  const olc::vi2d&
  CoordinateFrames::spriteSize() const noexcept {
    return m_ss;
  }

  inline
  olc::vi2d
  CoordinateFrames::spriteCoordsToPixels(int x, int y) const noexcept {
    return olc::vi2d(x * m_ss.x, y * m_ss.y);
  }

  inline
  const olc::vi2d&
  CoordinateFrames::tileSize() const noexcept {
    return m_ts;
  }

  inline
  olc::vi2d
  CoordinateFrames::tileCoordsToPixels(int x, int y) const noexcept {
    return olc::vi2d(
      m_wo.x + (y - x) * m_ts.x / 2,
      m_wo.y + (x + y) * m_ts.y / 2 - m_ts.y
    );
  }

  inline
  olc::vi2d
  CoordinateFrames::pixelCoordsToTiles(const olc::vi2d& pixels, int& q, olc::vi2d& tc) const noexcept {
    // Depending on whether we're in the negative realm or
    // in the positive one we need to compute the grid cell
    // index differently.
    int pox = pixels.x - m_wo.x;
    int poy = pixels.y - m_wo.y;

    int tx = (pox >= 0 ? pox / m_ts.x : pox / m_ts.x - 1);
    int ty = (poy >= 0 ? poy / m_ts.y : poy / m_ts.y - 1);

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
    // the tile itself. Note that we should add the
    // offset due to the world's origin to the pix
    // coords and *then* compute the offset in the
    // tile.
    // The last `+ m_ts.x) % m_ts.x)` is to make
    // sure that if the pixel position is before
    // the world origin (so `pixels.x < m_wo.x`) we
    // still end up with a positive value.
    int x = ((pixels.x - m_wo.x) % m_ts.x + m_ts.x) % m_ts.x;
    int y = ((pixels.y - m_wo.y) % m_ts.y + m_ts.y) % m_ts.y;

    tc = olc::vi2d(x, y);

    float hw = m_ts.x / 2.0f;
    float hh = m_ts.y / 2.0f;
    float how = 1.0f * m_ts.y / m_ts.x;

    // Now detect each corner and adjust the coordinate
    // of the cell.
    q = 0;
    if (x < hw && y < hh && y < hh - x * how) {
      // Top left corner.
      --rt.y;
      q += 1;
    }
    if (x > hw && y < hh && y < x * how - hh) {
      // Top right corner.
      --rt.x;
      q += 10;
    }
    if (x < hw && y > hh && y > hh + x * how) {
      // Bottom left corner.
      ++rt.x;
      q += 100;
    }
    if (x > hw && y > hh && y > 3 * hh - x * how) {
      // Bottom right corner.
      ++rt.y;
      q += 1000;
    }

    return rt;
  }

}

#endif    /* COORDINATE_FRAMES_HXX */

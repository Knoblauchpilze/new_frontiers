#ifndef    COORDINATE_FRAMES_HXX
# define   COORDINATE_FRAMES_HXX

# include "CoordinateFrames.hh"

namespace new_frontiers {

  inline
  CoordinateFrames::CoordinateFrames(const olc::vi2d& worldOrigin,
                                     const olc::vi2d& sprite,
                                     const olc::vi2d& tile):
    m_wox(worldOrigin.x),
    m_woy(worldOrigin.y),

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
      m_wox * m_ts.x + (y - x) * m_ts.x / 2,
      m_woy * m_ts.y + (x + y) * m_ts.y / 2 - m_ts.y
    );
  }

  inline
  olc::vi2d
  CoordinateFrames::pixelCoordsToTiles(const olc::vi2d& pixels) const noexcept {
    int tx = pixels.x / m_ts.x - m_wox;
    int ty = pixels.y / m_ts.y - m_woy;

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
    int x = pixels.x % m_ts.x;
    int y = pixels.y % m_ts.y;

    float hw = m_ts.x / 2.0f;
    float hh = m_ts.y / 2.0f;
    float how = m_ts.y / m_ts.x;

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

}

#endif    /* COORDINATE_FRAMES_HXX */

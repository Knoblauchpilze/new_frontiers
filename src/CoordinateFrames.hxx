#ifndef    COORDINATE_FRAMES_HXX
# define   COORDINATE_FRAMES_HXX

# include "CoordinateFrames.hh"
# include <cmath>

namespace new_frontiers {

  inline
  const olc::vf2d&
  CoordinateFrames::tileScale() const noexcept {
    return m_scale;
  }

  inline
  void
  CoordinateFrames::beginTranslation(const olc::vi2d& origin) {
    m_translationOrigin = origin;
    m_cachedPOrigin = m_pViewport.p;
  }

  inline
  void
  CoordinateFrames::translate(const olc::vi2d& pos) {
    // We need to deduce the translation added by
    // the input `pos` assuming that this will be
    // the final position of the viewport.
    olc::vf2d translation = pos - m_translationOrigin;
    m_pViewport.p = m_cachedPOrigin + translation;
  }

  inline
  void
  CoordinateFrames::zoom(const Zoom& z, const olc::vf2d& pos) {
    // Compute the factor to apply to the viewports
    // based on the operation to perform. We will
    // assume a zoom in behavior and update if need
    // be.
    float factor = 0.5f;
    if (z == Zoom::Out) {
      factor = 2.0f;
    }

    // What we know is that the top left cell of
    // the viewport is at the top left corner of
    // the pixels viewport.
    // We can compute the distance from the `pos`
    // to the pixels viewport origin: it should
    // be applied the *inverse* of the `factor`
    // and we should be good to go: indeed if a
    // viewport has its span reduced, distances
    // are lengthened (and conversely).
    olc::vf2d d = m_pViewport.p - pos;
    d /= factor;

    m_pViewport.p = pos + d;

    // Also update the dimensions of the cells
    // viewport by `factor`.
    m_cViewport.dims *= factor;

    updateTileScale();
  }

  inline
  olc::vf2d
  CoordinateFrames::tileCoordsToPixels(int x, int y) const noexcept {
    // Offset the input coordinates based on the
    // current position of the cell's viewport.
    x -= m_cViewport.p.x;
    y -= m_cViewport.p.y;

    // The isomectric representation yields the
    // formula below which takes into account a
    // scaling factor to apply to the tiles.
    return olc::vf2d(
      m_pViewport.p.x + (y - x) * m_tScaled.x / 2,
      m_pViewport.p.y + (x + y) * m_tScaled.y / 2
    );
  }

  inline
  olc::vi2d
  CoordinateFrames::pixelCoordsToTiles(const olc::vi2d& pixels) const noexcept {
    // The conversion to float value is necessary in the
    // case of negative values where for example coords
    // `(-0.5, -0.5)` should be interpreted as belonging
    // to the cell `(-1, -1)`.
    float pox = pixels.x - m_pViewport.p.x;
    float poy = pixels.y - m_pViewport.p.y - m_tScaled.y;

    int tx = static_cast<int>(std::floor(pox / m_tScaled.x));
    int ty = static_cast<int>(std::floor(poy / m_tScaled.y));

    /** TODO: Issue with the cViewport top left corner. **/
    tx -= m_cViewport.p.x;
    ty -= m_cViewport.p.y;

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
    // The last `+ m_tScaled.x) % m_tScaled.x)` is
    // to make sure that if the pixel position is
    // before the actual position of the top left
    // cell of the viewport (so we have this:
    // `pixels.x < m_pViewport.p.x`) we still end
    // up with a positive value.
    float x = std::fmod(std::fmod(pox, m_tScaled.x) + m_tScaled.x, m_tScaled.x);
    float y = std::fmod(std::fmod(poy, m_tScaled.y) + m_tScaled.y, m_tScaled.y);

    float hw = m_tScaled.x / 2.0f;
    float hh = m_tScaled.y / 2.0f;
    float how = 1.0f * m_tScaled.y / m_tScaled.x;

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

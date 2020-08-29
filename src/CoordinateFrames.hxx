#ifndef    COORDINATE_FRAMES_HXX
# define   COORDINATE_FRAMES_HXX

# include "CoordinateFrames.hh"
# include <cmath>
# include "utils.hh"

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
    // the final position of the world's origin.
    olc::vi2d translation = pos - m_translationOrigin;
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

    log("Zoomed from " + toString(m_cViewport.p) + " d: " + toString(m_cViewport.dims));
    log("Factor is " + std::to_string(factor));

    // Convert the input position to cells coords.
    int q; olc::vi2d gc; olc::vf2d to;
    olc::vi2d p = pixelCoordsToTiles(pos, q, gc, to);

    // We want to reduce or expand the viewport by
    // the specified factor around the provided pos.
    // We need to compute the new position of the
    // top left corner of the viewport.

    m_cViewport.dims *= factor;
    m_cViewport.p = p - m_cViewport.dims / 2.0f;

    updateTileScale();

    log("Mouse is " + toString(pos) + " and tiles " + toString(p));
    log("New is " + toString(m_cViewport.p) + " d: " + toString(m_cViewport.dims));
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
    return olc::vf2d((y - x) * m_tScaled.x / 2, (x + y) * m_tScaled.y / 2);
  }

  inline
  olc::vi2d
  CoordinateFrames::pixelCoordsToTiles(const olc::vi2d& pixels, int& q, olc::vi2d& gc, olc::vf2d& to) const noexcept {
    // The conversion to float value is necessary in the
    // case of negative values where for example coords
    // `(-0.5, -0.5)` should be interpreted as belonging
    // to the cell `(-1, -1)`.
    int tx = static_cast<int>(std::floor(1.0f * pixels.x / m_tScaled.x));
    int ty = static_cast<int>(std::floor((1.0f * pixels.y - m_ts.y) / m_tScaled.y));

    olc::vi2d rt(ty - tx, ty + tx);

    gc = olc::vf2d(tx, ty);

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
    float x = std::fmod(1.0f * pixels.x, m_tScaled.x);
    float y = std::fmod(1.0f * pixels.y, m_tScaled.y);

    to = olc::vf2d(x, y);

    float hw = m_tScaled.x / 2.0f;
    float hh = m_tScaled.y / 2.0f;
    float how = 1.0f * m_tScaled.y / m_tScaled.x;

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

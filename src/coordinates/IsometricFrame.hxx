#ifndef    ISOMETRIC_FRAME_HXX
# define   ISOMETRIC_FRAME_HXX

# include "IsometricFrame.hh"

namespace new_frontiers {

  inline
  IsometricFrame::IsometricFrame(const Viewport& cvp,
                                 const Viewport& pvp,
                                 const olc::vi2d& tileSize):
    CoordinateFrame(cvp, pvp, tileSize)
  {}

  inline
  Viewport
  IsometricFrame::cellsViewport() const noexcept {
    // We know the position of the origin point
    // in pixels. We also know the size of each
    // cell in pixels.
    // From there we can convert the position
    // `(0, 0)` in pixels to tiles. Note that as
    // we actually have isometric projection we
    // need to project all four corners:
    // the top left and bottom right will give
    // the extent in `y` coordinate while the
    // bottom left and top right will give the
    // extent in the `x` coordinate.
    olc::vi2d tl = pixelCoordsToTiles(olc::vi2d(0, 0));
    olc::vi2d tr = pixelCoordsToTiles(olc::vi2d(m_pViewport.dims.x, 0));
    olc::vi2d br = pixelCoordsToTiles(olc::vi2d(m_pViewport.dims.x, m_pViewport.dims.y));
    olc::vi2d bl = pixelCoordsToTiles(olc::vi2d(0, m_pViewport.dims.y));

    Viewport out;
    out.p = olc::vf2d(tr.x, tl.y);
    // The `+1` comes from the fact that the
    // difference between corners account for
    // the tiles in between corners but not
    // the `tr.x` tile.
    // It's similar to the well-known example:
    // `how many integer numbers are there in
    // the range [0; 2] ?`
    // The answer is not `2 - 0 = 2` but `3`
    // as `2 - 0` only counts the number that
    // are greater than the lower bound and
    // note the lower bound itself.
    out.dims = olc::vf2d(bl.x - tr.x + 1, br.y - tl.y + 1);

    return out;
  }

  inline
  olc::vf2d
  IsometricFrame::tileCoordsToPixels(float x, float y, float radius, const Cell& pos) const noexcept {
    // Offset the input coordinates based on the
    // current position of the cell's viewport.
    x -= m_cViewport.p.x;
    y -= m_cViewport.p.y;

    // The isometric representation yields the
    // formula below which takes into account a
    // scaling factor to apply to the tiles.
    olc::vf2d tp(
      m_pViewport.p.x + (y - x) * m_tScaled.x / 2.0f,
      m_pViewport.p.y + (x + y) * m_tScaled.y / 2.0f
    );

    // Now that we have the top left coordinate
    // of the cell we can adjust based on the
    // display mode desired by the user.
    switch (pos) {
      case Cell::CenterBottom:
        tp.x += (m_tScaled.x / 2.0f);
        tp.y += m_tScaled.y;
        break;
      case Cell::UpperLeft:
        tp.x += (m_tScaled.x / 2.0f - radius * m_tScaled.x / 2.0f);
        tp.y += (m_tScaled.y - 2.0f * radius * m_tScaled.y);
        break;
      case Cell::UpperEast:
        tp.x -= (m_tScaled.x / 2.0f);
        break;
      case Cell::BottomLeft:
        tp.x += (m_tScaled.x / 2.0f - radius * m_tScaled.x / 2.0f);
        tp.y += m_tScaled.y;
        break;
      case Cell::UpperTopLeft:
        tp.x -= (m_tScaled.x / 2.0f);
        tp.y += (m_tScaled.y - 3.0f * radius * m_tScaled.y);
        break;
      case Cell::TopLeft:
      default:
        break;
    }

    return tp;
  }

  inline
  olc::vi2d
  IsometricFrame::pixelCoordsToTiles(const olc::vi2d& pixels, olc::vf2d* intraTile) const noexcept {
    // The conversion to float value is necessary in the
    // case of negative values where for example coords
    // `(-0.5, -0.5)` should be interpreted as belonging
    // to the cell `(-1, -1)`.
    float pox = pixels.x - m_pViewport.p.x;
    float poy = pixels.y - m_pViewport.p.y - m_tScaled.y;

    int tx = static_cast<int>(std::floor(pox / m_tScaled.x));
    int ty = static_cast<int>(std::floor(poy / m_tScaled.y));

    olc::vi2d rt(ty - tx, ty + tx);

    // The viewport should be handled after we first
    // computed coordinates in the world's frame: it
    // is what is meant by the cells' offset.
    rt.x += m_cViewport.p.x;
    rt.y += m_cViewport.p.y;

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

    olc::vf2d topLeftTile = tileCoordsToPixels(rt.x, rt.y, 1.0f, Cell::TopLeft);
    olc::vf2d intraTileBase(
      (pixels.x - topLeftTile.x) / m_tScaled.x,
      (pixels.y - topLeftTile.y - m_tScaled.y) / m_tScaled.y
    );

    if (intraTile != nullptr) {
      intraTile->x = -0.5f;
      intraTile->y = 0.0f;

      // When increasing the `x` screen coordinates,
      // the `x` intra tile coordinate is decreasing
      // continuously from `1` to `0`.
      // The `y` intra tile coordinate on the other
      // hand is increasing.
      intraTile->x += (1.0f - intraTileBase.x);
      intraTile->y += (intraTileBase.x - 0.5f);

      // When increasing the `y` screen coordinate,
      // the `y` intra tile coordinate is also
      // increasing in the same proportion. Initially
      // the value is `0` as well.
      // The `x` intra tile coordinate also varies
      // in the same measure.
      intraTile->x += intraTileBase.y;
      intraTile->y += intraTileBase.y;
    }

    float hw = m_tScaled.x / 2.0f;
    float hh = m_tScaled.y / 2.0f;
    float how = 1.0f * m_tScaled.y / m_tScaled.x;

    // Now detect each corner and adjust the coordinate
    // of the cell.
    if (x < hw && y < hh && y < hh - x * how) {
      // Top left corner.
      --rt.y;

      if (intraTile != nullptr) {
        intraTile->y +=1.0f;
      }
    }
    if (x > hw && y < hh && y < x * how - hh) {
      // Top right corner.
      --rt.x;

      if (intraTile != nullptr) {
        intraTile->x += 1.0f;
      }
    }
    if (x < hw && y > hh && y > hh + x * how) {
      // Bottom left corner.
      ++rt.x;

      if (intraTile != nullptr) {
        intraTile->x -= 1.0f;
      }
    }
    if (x > hw && y > hh && y > 3 * hh - x * how) {
      // Bottom right corner.
      ++rt.y;

      if (intraTile != nullptr) {
        intraTile->y -= 1.0f;
      }
    }

    return rt;
  }

}

#endif    /* ISOMETRIC_FRAME_HXX */

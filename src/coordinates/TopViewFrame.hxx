#ifndef    TOPVIEW_FRAME_HXX
# define   TOPVIEW_FRAME_HXX

# include "TopViewFrame.hh"

namespace new_frontiers {

  inline
  TopViewFrame::TopViewFrame(const Viewport& cvp,
                             const Viewport& pvp,
                             const olc::vi2d& tileSize):
    CoordinateFrame(cvp, pvp, tileSize)
  {}

  inline
  olc::vf2d
  TopViewFrame::tileCoordsToPixels(float x, float y, float radius, const Cell& pos) const noexcept {
    // Offset the input coordinates based on the
    // current position of the cell's viewport.
    x -= m_cViewport.p.x;
    y -= m_cViewport.p.y;

    // Convert to top view coordinates: we just
    // need to scale by the tile size.
    olc::vf2d tp(
      m_pViewport.p.x + x * m_tScaled.x,
      m_pViewport.p.y + y * m_tScaled.y
    );

    // Now that we have the top left coordinate
    // of the cell we can adjust based on the
    // display mode desired by the user.
    switch (pos) {
      case Cell::CenterBottom:
        tp.x += radius * m_tScaled.x / 2.0f;
        tp.y += radius * m_tScaled.y;
        break;
      case Cell::UpperLeft:
        tp.y -= radius * m_tScaled.y / 2.0f;
        break;
      case Cell::UpperEast:
        tp.x -= radius * m_tScaled.x / 2.0f;
        break;
      case Cell::BottomLeft:
        tp.y += radius * m_tScaled.y;
        break;
      case Cell::UpperTopLeft:
        tp.x -= radius * m_tScaled.x / 2.0f;
        tp.y -= radius * m_tScaled.y;
        break;
      case Cell::TopLeft:
      default:
        break;
    }

    return tp;
  }

  inline
  olc::vi2d
  TopViewFrame::pixelCoordsToTiles(const olc::vi2d& pixels, olc::vf2d* intraTile) const noexcept {
    // The conversion to float value is necessary in the
    // case of negative values where for example coords
    // `(-0.5, -0.5)` should be interpreted as belonging
    // to the cell `(-1, -1)`.
    float pox = pixels.x - m_pViewport.p.x;
    float poy = pixels.y - m_pViewport.p.y;

    int tx = static_cast<int>(std::floor(pox / m_tScaled.x));
    int ty = static_cast<int>(std::floor(poy / m_tScaled.y));

    olc::vi2d rt(tx, ty);

    // The viewport should be handled after we first
    // computed coordinates in the world's frame: it
    // is what is meant by the cells' offset.
    rt.x += m_cViewport.p.x;
    rt.y += m_cViewport.p.y;

    // Compute the intra-tile offset by converting
    // the tile position to pixels and comparing the
    // input value.
    olc::vf2d tlp = tileCoordsToPixels(rt.x, rt.y, 1.0f, Cell::TopLeft);

    if (intraTile != nullptr) {
      intraTile->x = pixels.x - tlp.x;
      intraTile->y = pixels.y - tlp.y;

      // Convert the intra tile to value in the range
      // `[0; 1]`.
      intraTile->x /= m_tScaled.x;
      intraTile->y /= m_tScaled.y;
    }

    return rt;
  }

}

#endif    /* TOPVIEW_FRAME_HXX */

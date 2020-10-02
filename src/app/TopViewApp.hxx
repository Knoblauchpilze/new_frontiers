#ifndef    TOPVIEW_APP_HXX
# define   TOPVIEW_APP_HXX

# include "TopViewApp.hh"
# include "utils.hh"

namespace new_frontiers {

  inline
  void
  TopViewApp::cleanResources() {
    // Nothing to do for now.
  }

  inline
  void
  TopViewApp::drawSprite(const SpriteDesc& tile, const CoordinateFrame& cf) {
    olc::Pixel c = m_sprites[tile.type];
    c.a = tile.alpha;

    FillRectDecal(
      cf.tileCoordsToPixels(tile.x, tile.y, tile.radius, tile.location),
      tile.radius * cf.tileSize(),
      c
    );
  }

}

#endif    /* TOPVIEW_APP_HXX */

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

  inline
  void
  TopViewApp::drawHealthBar(const SpriteDesc& tile, float ratio, const CoordinateFrame& cf) {
    // Fetch a color based on the input ratio.
    olc::Pixel hbc = redToGreenGradient(ratio, tile.alpha);

    // Darken it for the empty portion of the
    // healthbar.
    olc::Pixel bc(
      static_cast<int>(hbc.r * 0.5f),
      static_cast<int>(hbc.g * 0.5f),
      static_cast<int>(hbc.b * 0.5f),
      hbc.a
    );

    // Draw both parts of the healthbar. The
    // combined length of both elements will
    // be placed close to the position of the
    // actual element and have a size of 7/10th
    // of a tile in width, and 1/10th of a tile
    // in height.
    float hbWRatio = 0.7f;
    float hbHRatio = 0.1f;
    float hbHOffset = 0.1f;
    olc::vf2d s = cf.tileSize();

    FillRectDecal(
      olc::vf2d(tile.x + (1.0f - hbWRatio) * s.x / 2.0f, tile.y - s.y * hbHOffset),
      olc::vf2d(s.x * hbWRatio * ratio, s.y * hbHRatio),
      hbc
    );
    FillRectDecal(
      olc::vf2d(tile.x + (1.0f - hbWRatio) * s.x / 2.0f + s.x * hbWRatio * ratio, tile.y - s.y * hbHOffset),
      olc::vf2d(s.x * hbWRatio * (1.0f - ratio), s.y * hbHRatio),
      bc
    );
  }

}

#endif    /* TOPVIEW_APP_HXX */

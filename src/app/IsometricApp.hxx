#ifndef    ISOMETRIC_APP_HXX
# define   ISOMETRIC_APP_HXX

# include "IsometricApp.hh"

namespace new_frontiers {

  inline
  void
  IsometricApp::cleanResources() {
    // Clear game sprites.
    if (!m_sprites.empty()) {
      for (unsigned id = 0u ; id < m_sprites.size() ; ++id) {
        if (m_sprites[id].res != nullptr) {
          delete m_sprites[id].res;
        }
      }

      m_sprites.clear();
    }
  }

  inline
  olc::vi2d
  IsometricApp::spriteCoordsToPixels(const olc::vi2d& coord, const olc::vi2d& layout, int id) const noexcept {
    // Compute linear identifier from 2D coordinates.
    int lID = coord.y * layout.x + coord.x + id;

    // Go back to 2D coordinates using the layout on
    // the linearized ID and the size of the sprite
    // to obtain a pixels position.
    return olc::vi2d(
      (lID % layout.x) * m_ss.x,
      (lID / layout.x) * m_ss.y
    );
  }

  inline
  int
  IsometricApp::aliasOfBlock(const tiles::Block& block) {
    return block;
  }

  inline
  int
  IsometricApp::aliasOfEntity(const tiles::Entity& ent) {
    return tiles::BlocksCount + ent;
  }

  inline
  int
  IsometricApp::aliasOfEffect(const tiles::Effect& vfx) {
    return tiles::BlocksCount + tiles::EntitiesCount + vfx;
  }

  inline
  void
  IsometricApp::drawSprite(const olc::vf2d& pos, const olc::vf2d& tileScale, int alias, int id, int alpha) {
    const SpriteAlias& sa = m_aliases[alias];
    const SpritesPack& sp = m_sprites[sa.type];

    DrawPartialDecal(
      pos,
      sp.res,
      spriteCoordsToPixels(sa.alias, sp.layout, id),
      m_ss,
      tileScale,
      olc::Pixel(255, 255, 255, alpha)
    );
  }

  inline
  void
  IsometricApp::drawHealthBar(const olc::vf2d& pos, const olc::vf2d& tileScale, float ratio, int alpha) {
    // Fetch a color based on the input ratio.
    olc::Pixel hbc = redToGreenGradient(ratio, alpha);

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
    olc::vf2d s = m_ss * tileScale;

    FillRectDecal(
      olc::vf2d(pos.x + (1.0f - hbWRatio) * s.x / 2.0f, pos.y - s.y * hbHOffset),
      olc::vf2d(s.x * hbWRatio * ratio, s.y * hbHRatio),
      hbc
    );
    FillRectDecal(
      olc::vf2d(pos.x + (1.0f - hbWRatio) * s.x / 2.0f + s.x * hbWRatio * ratio, pos.y - s.y * hbHOffset),
      olc::vf2d(s.x * hbWRatio * (1.0f - ratio), s.y * hbHRatio),
      bc
    );
  }

}

#endif    /* ISOMETRIC_APP_HXX */

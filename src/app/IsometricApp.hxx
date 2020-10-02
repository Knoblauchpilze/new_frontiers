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
  IsometricApp::drawSprite(const SpriteDesc& tile,
                           const CoordinateFrame& cf,
                           int id)
  {
    const SpriteAlias& sa = m_aliases[tile.type];
    const SpritesPack& sp = m_sprites[sa.type];

    DrawPartialDecal(
      cf.tileCoordsToPixels(tile.x, tile.y, tile.radius, tile.location),
      sp.res,
      spriteCoordsToPixels(sa.alias, sp.layout, id),
      m_ss,
      tile.radius * cf.tileScale(),
      olc::Pixel(255, 255, 255, tile.alpha)
    );
  }

}

#endif    /* ISOMETRIC_APP_HXX */

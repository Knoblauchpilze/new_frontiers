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
    return static_cast<int>(tiles::BlocksCount) + ent;
  }

  inline
  int
  IsometricApp::aliasOfEffect(const tiles::Effect& vfx) {
    return static_cast<int>(tiles::BlocksCount) + static_cast<int>(tiles::EntitiesCount) + vfx;
  }

  inline
  void
  IsometricApp::drawSprite(const SpriteDesc& tile,
                           const CoordinateFrame& cf,
                           int id,
                           const olc::Pixel& tint)
  {
    const SpriteAlias& sa = m_aliases[tile.type];
    const SpritesPack& sp = m_sprites[sa.type];

    DrawPartialDecal(
      cf.tileCoordsToPixels(tile.x, tile.y, tile.radius, tile.location),
      sp.res,
      spriteCoordsToPixels(sa.alias, sp.layout, id),
      m_ss,
      tile.radius * cf.tileScale(),
      olc::Pixel(tint.r, tint.g, tint.b, tile.alpha)
    );
  }

  inline
  olc::Pixel
  IsometricApp::getColorFor(const std::string& colony) noexcept {
    // Fetch from exsisting.
    ColorMap::const_iterator it = m_coloniesColors.find(colony);
    if (it != m_coloniesColors.cend()) {
      return it->second;
    }

    // Otherwise generate a new one.
    bool failed;
    olc::Pixel col = m_cGenerator.generate(colony, failed);

    if (failed) {
      debug("Failed to generate color for \"" + colony + "\"");
      col = olc::WHITE;
    }
    else {
      m_coloniesColors[colony] = col;
    }

    return col;
}

}

#endif    /* ISOMETRIC_APP_HXX */

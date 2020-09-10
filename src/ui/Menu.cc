
# include "Menu.hh"
# include "../utils.hh"

namespace new_frontiers {

  void
  Menu::render(olc::PixelGameEngine* pge) const {
    // We either need to fill the area for this menu
    // with the uniform color or perform the tiling
    // with the background decal if needed.
    if (m_bgSprite == nullptr) {
      pge->FillRectDecal(m_pos, m_size);

      return;
    }

    // Define the scale to apply to the sprite in
    // order to match the expected tiling.
    olc::vi2d sDims(m_bgSprite->sprite->width, m_bgSprite->sprite->height);
    const olc::vi2d& bgWarp = m_bg.warp;

    olc::vf2d repeat(1.0f * m_size.x / bgWarp.x, 1.0f * m_size.y / bgWarp.y);
    olc::vf2d s(1.0f * bgWarp.x / sDims.x, 1.0f * bgWarp.y / sDims.y);

    olc::vf2d o;
    float xR;

    // Repeatedly blit the sprite on the background.
    while (repeat.y >= 1.0f) {
      xR = repeat.x;
      o.x = 0.0f;

      while (xR >= 1.0f) {
        pge->DrawDecal(
          m_pos + o,
          m_bgSprite,
          s
        );

        o.x += bgWarp.x;
        xR -= 1.0f;
      }
        
      if (xR > 0.0f) {
        pge->DrawPartialDecal(
          m_pos + o,
          olc::vf2d(bgWarp.x * xR, bgWarp.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x * xR, sDims.y)
        );
      }

      o.y += bgWarp.y;
      repeat.y -= 1.0f;
    }

    if (repeat.y > 0.0f) {
      xR = repeat.x;
      o.x = 0.0f;

      while (xR >= 1.0f) {
        pge->DrawPartialDecal(
          m_pos + o,
          olc::vf2d(bgWarp.x, bgWarp.y * repeat.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x, sDims.y * repeat.y)
        );

        o.x += bgWarp.x;
        xR -= 1.0f;
      }

      if (xR > 0.0f) {
        pge->DrawPartialDecal(
          m_pos + o,
          olc::vf2d(bgWarp.x * xR, bgWarp.y * repeat.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x * xR, sDims.y * repeat.y)
        );
      }
    }
  }

  void
  Menu::loadBGTile() {
    // In case the background is not tiled, nothing
    // to be loaded.
    if (m_bg.sprite == "") {
      return;
    }

    // Make sure that the tiling value is valid.
    m_bg.warp.x = std::max(m_bg.warp.x, 10);
    m_bg.warp.y = std::max(m_bg.warp.y, 10);

    // Load the sprite.
    olc::Sprite* spr = new olc::Sprite(m_bg.sprite);
    m_bgSprite = new olc::Decal(spr);
  }

}

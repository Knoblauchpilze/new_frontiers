
# include "Menu.hh"
# include "../utils.hh"

namespace new_frontiers {

  Menu::Menu(const olc::vi2d& pos,
             const olc::vf2d& size,
             const std::string& name,
             const Layout& layout,
             Menu* parent):
    utils::CoreObject(name),

    m_pos(pos),
    m_size(size),

    m_bg(newColoredBackground(olc::Pixel(255, 255, 255, 0))),
    m_bgSprite(nullptr),

    m_layout(layout),

    m_parent(parent),
    m_children()
  {}

  void
  Menu::render(olc::PixelGameEngine* pge) const {
    // We either need to fill the area for this menu
    // with the uniform color or perform the tiling
    // with the background decal if needed.
    olc::vi2d pos = absolutePosition();

    if (m_bgSprite == nullptr) {
      pge->FillRectDecal(pos, m_size);

      return;
    }

    // Define the scale to apply to the sprite in
    // order to match the expected tiling.
    olc::vi2d sDims(m_bgSprite->sprite->width, m_bgSprite->sprite->height);
    const olc::vi2d& bgWrap = m_bg.wrap;

    olc::vf2d repeat(1.0f * m_size.x / bgWrap.x, 1.0f * m_size.y / bgWrap.y);
    olc::vf2d s(1.0f * bgWrap.x / sDims.x, 1.0f * bgWrap.y / sDims.y);

    olc::vf2d o;
    float xR;

    // TODO: Handle repeat.
    // TODO: Add labels.
    // TODO: Add icons for menu.
    // TODO: Maybe two subclasses ?

    // Repeatedly blit the sprite on the background.
    while (repeat.y >= 1.0f) {
      xR = repeat.x;
      o.x = 0.0f;

      while (xR >= 1.0f) {
        pge->DrawDecal(
          pos + o,
          m_bgSprite,
          s
        );

        o.x += bgWrap.x;
        xR -= 1.0f;
      }
        
      if (xR > 0.0f) {
        pge->DrawPartialDecal(
          pos + o,
          olc::vf2d(bgWrap.x * xR, bgWrap.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x * xR, sDims.y)
        );
      }

      o.y += bgWrap.y;
      repeat.y -= 1.0f;
    }

    if (repeat.y > 0.0f) {
      xR = repeat.x;
      o.x = 0.0f;

      while (xR >= 1.0f) {
        pge->DrawPartialDecal(
          pos + o,
          olc::vf2d(bgWrap.x, bgWrap.y * repeat.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x, sDims.y * repeat.y)
        );

        o.x += bgWrap.x;
        xR -= 1.0f;
      }

      if (xR > 0.0f) {
        pge->DrawPartialDecal(
          pos + o,
          olc::vf2d(bgWrap.x * xR, bgWrap.y * repeat.y),
          m_bgSprite,
          olc::vf2d(0.0f, 0.0f),
          olc::vf2d(sDims.x * xR, sDims.y * repeat.y)
        );
      }
    }

    // Render this menu.
    renderSelf(pge);

    // And then draw children in the order there were
    // added: it means that the last added menu will
    // be repainted on top of the others.
    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      m_children[id]->render(pge);
    }
  }

  void
  Menu::addMenu(MenuShPtr child) {
    // Check consistency.
    if (child == nullptr) {
      return;
    }

    // Assign this element has the parent of the new
    // child.
    child->m_parent = this;

    // Update the size based on the layout for this
    // menu: we also have to update the other items
    // so that a consistent size is defined.
    // Note that we only assign the perpendicular
    // size to the menu layout's direction so that
    // we keep the actual desired space for the
    // child in the principal direction.
    int d = (m_layout == Layout::Horizontal ? m_size.y : m_size.x);

    m_children.push_back(child);

    // Note that we also force the wraping for the
    // background tiled children to something that
    // will be consistent with their actual size.
    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      switch (m_layout) {
        case Layout::Vertical:
          m_children[id]->m_size.x = d;
          break;
        case Layout::Horizontal:
        default:
          m_children[id]->m_size.y = d;
          break;
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
    m_bg.wrap.x = std::max(m_bg.wrap.x, 10);
    m_bg.wrap.y = std::max(m_bg.wrap.y, 10);

    // Load the sprite.
    olc::Sprite* spr = new olc::Sprite(m_bg.sprite);
    m_bgSprite = new olc::Decal(spr);
  }

}

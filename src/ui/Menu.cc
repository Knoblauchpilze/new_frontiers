
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
    // m_content(newTextContent("default menu", Alignment::Left)),
    m_content(newMenuContent("default menu", "data/img/menu/minimap.png", olc::vi2d(300, 100))),
    m_mcSprite(nullptr),

    m_layout(layout),

    m_parent(parent),
    m_children()
  {
    loadContentTile();
  }

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
    // child and register it as a new submenu.
    child->m_parent = this;

    m_children.push_back(child);

    // Update properties of each child in response
    // to the new child.
    updateChildren();
  }

  void
  Menu::renderSelf(olc::PixelGameEngine* pge) const {
    // We need to display both the text and the icon
    // if needed. We assume the content will always
    // be centered along the perpendicular axis for
    // this menu.
    switch (m_layout) {
      case Layout::Vertical:
        break;
      case Layout::Horizontal:
      default:
        break;
    }

    // Depending on whether we need to display both
    // an image or a text and in which order layout
    // in the menu will change.
    olc::vi2d ap = absolutePosition();


    if (m_content.text != "" && m_mcSprite == nullptr) {
      olc::vi2d ts = pge->GetTextSize(m_content.text);

      olc::vi2d p;
      switch (m_content.align) {
        case Alignment::Center:
          p = olc::vi2d(
            static_cast<int>(ap.x + (m_size.x - ts.x) / 2.0f),
            static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
          );
          break;
        case Alignment::Right:
          p = olc::vi2d(
            ap.x + m_size.x - ts.x,
            static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
          );
          break;
        case Alignment::Left:
        default:
          p = olc::vi2d(
            ap.x,
            static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
          );
          break;
      }

      pge->DrawStringDecal(
        p,
        m_content.text,
        olc::MAGENTA
      );
    }

    if (m_content.text == "" && m_mcSprite != nullptr) {
      // Center the image if it is the only element
      // to display.
      olc::vi2d p(
        std::max(static_cast<int>(ap.x + m_size.x / 2.0f - m_content.size.x / 2.0f), 0),
        std::max(static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f), 0)
      );

      olc::vi2d ss(m_mcSprite->sprite->width, m_mcSprite->sprite->height);
      olc::vf2d s(1.0f * m_content.size.x / ss.x, 1.0f * m_content.size.y / ss.y);

      pge->DrawPartialDecal(
        p,
        m_mcSprite,
        olc::vi2d(),
        ss,
        s
      );
    }

    // Both text and icon should be displayed.
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

  void
  Menu::loadContentTile() {
    // Check for actually needing to load anything.
    if (m_content.icon == "") {
      return;
    }

    // Clamp the size of the icon.
    m_content.size.x = std::max(m_content.size.x, 10);
    m_content.size.y = std::max(m_content.size.y, 10);

    // Load the sprite.
    olc::Sprite* spr = new olc::Sprite(m_content.icon);
    m_mcSprite = new olc::Decal(spr);
  }

  void
  Menu::updateChildren() {
    // Update the size based on the layout for this
    // menu: we also have to update the other items
    // so that a consistent size is defined.
    // Note that we only assign the perpendicular
    // size to the menu layout's direction so that
    // we keep the actual desired space for the
    // child in the principal direction.
    int wh = (m_layout == Layout::Horizontal ? m_size.y : m_size.x);
    int i = (m_layout == Layout::Horizontal ? m_size.x : m_size.y);
    int d = static_cast<int>(1.0f * i / m_children.size());

    // Note that we also force the wraping for the
    // background tiled children to something that
    // will be consistent with their actual size.
    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      switch (m_layout) {
        case Layout::Vertical:
          m_children[id]->m_size.x = wh;
          m_children[id]->m_size.y = d;

          m_children[id]->m_pos.x = 0;
          m_children[id]->m_pos.y = id * d;

          // Update the wrap size for the background of
          // the child menu if it indicates a no-wrap
          // behavior.
          if (!m_children[id]->m_bg.tiling) {
            m_children[id]->m_bg.wrap.x = wh;
            m_children[id]->m_bg.wrap.y = d;
          }
          break;
        case Layout::Horizontal:
        default:
          m_children[id]->m_size.x = d;
          m_children[id]->m_size.y = wh;

          m_children[id]->m_pos.x = id * d;
          m_children[id]->m_pos.y = 0;

          if (!m_children[id]->m_bg.tiling) {
            m_children[id]->m_bg.wrap.x = d;
            m_children[id]->m_bg.wrap.y = wh;
          }
          break;
      }
    }
  }

}

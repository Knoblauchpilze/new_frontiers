
# include "Menu.hh"
# include "../utils.hh"

namespace new_frontiers {

  Menu::Menu(const olc::vi2d& pos,
             const olc::vf2d& size,
             const std::string& name,
             const Layout& layout,
             Menu* parent):
    utils::CoreObject(name),

    m_state(State{true, false, false}),

    m_pos(pos),
    m_size(size),

    m_bg(newColoredBackground(olc::Pixel(0, 255, 255, 0))),
    m_bgSprite(nullptr),
    m_content(newTextContent("default menu", Alignment::Left)),
    m_mcSprite(nullptr),

    m_layout(layout),

    m_parent(parent),
    m_children()
  {
    setService("menu");

    loadContentTile();
  }

  void
  Menu::render(olc::PixelGameEngine* pge) const {
    // If the menu is not visible, do nothing.
    if (!m_state.visible) {
      return;
    }

    // We either need to fill the area for this menu
    // with the uniform color or perform the tiling
    // with the background decal if needed.
    olc::vi2d pos = absolutePosition();

    if (m_bgSprite == nullptr) {
      pge->FillRectDecal(
        pos,
        m_size,
        (m_state.highlighted || m_state.selected ? m_bg.hColor : m_bg.color)
      );
    }

    if (m_bgSprite != nullptr) {
      // Define the scale to apply to the sprite in
      // order to match the expected tiling.
      olc::vi2d sDims(m_bgSprite->sprite->width, m_bgSprite->sprite->height);
      const olc::vi2d& bgWrap = m_bg.wrap;

      olc::vf2d repeat(1.0f * m_size.x / bgWrap.x, 1.0f * m_size.y / bgWrap.y);
      olc::vf2d s(1.0f * bgWrap.x / sDims.x, 1.0f * bgWrap.y / sDims.y);

      olc::vf2d o;
      float xR;

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

  Menu::InputHandle
  Menu::processUserInput(const controls::State& c,
                         std::vector<ActionShPtr>& actions)
  {
    // Make sure that the children get their chance
    // to process the event.
    InputHandle res{false, false};
    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      InputHandle rc = m_children[id]->processUserInput(c, actions);

      res.relevant = res.relevant || rc.relevant;
      res.selected = res.selected || rc.selected;
    }

    // If the mouse is not inside this element, stop
    // the process here: children still got a chance
    // to update their state with this event. And no
    // matter the `used` value, we know that we're
    // not highlighted anymore at this step if the
    // following conditions apply: it either mean
    // that the mouse is not inside this menu or
    // that a child is more relevant than we are.
    bool click = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);

    olc::vi2d ap = absolutePosition();
    if (c.mPosX < ap.x || c.mPosX >= ap.x + m_size.x ||
        c.mPosY < ap.y || c.mPosY >= ap.y + m_size.y ||
        res.relevant || res.selected)
    {
      m_state.highlighted = false;

      if (res.selected || click) {
        m_state.selected = false;
      }

      return res;
    }

    // This menu is now highlighted.
    m_state.highlighted = true;

    // In case the user clicks on the menu, we need
    // to trigger the corresponding handler.
    if (click) {
      onClick(actions);
      m_state.selected = true;
    }

    return res;
  }

  void
  Menu::addMenu(MenuShPtr child) {
    // Check consistency.
    if (child == nullptr) {
      return;
    }

    // Assign this element has the parent of the new
    // child and register it as a new submenu.
    // Note that this might cause issue in case the
    // child already has a parent.
    if (child->m_parent != nullptr) {
      log(
        "Reparenting menu \"" + child->getName() + "\" from \"" +
        child->m_parent->getName() + "\"",
        utils::Level::Warning
      );
    }

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
    // Depending on whether we need to display both
    // an image or a text and in which order layout
    // in the menu will change.
    // Also, in case there's no text nor sprite to
    // display we can return right now.
    if (m_content.text == "" && m_mcSprite == nullptr) {
      return;
    }

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
        m_content.color
      );

      return;
    }

    if (m_content.text == "" && m_mcSprite != nullptr) {
      // Center the image if it is the only element
      // to display.
      olc::vi2d p(
        static_cast<int>(ap.x + m_size.x / 2.0f - m_content.size.x / 2.0f),
        static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
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

      return;
    }

    // Both text and icon should be displayed: the
    // order is specified in the content description.
    olc::vi2d ts = pge->GetTextSize(m_content.text);
    olc::vi2d cs = ts + m_content.size;

    olc::vi2d tp;
    olc::vi2d sp;

    olc::vi2d ss(m_mcSprite->sprite->width, m_mcSprite->sprite->height);
    olc::vf2d s(1.0f * m_content.size.x / ss.x, 1.0f * m_content.size.y / ss.y);

    switch (m_content.order) {
      case Ordering::TextFirst:
        switch (m_content.align) {
          case Alignment::Center:
            tp = olc::vi2d(
              static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f),
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f + ts.x),
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
          case Alignment::Right:
            tp = olc::vi2d(
              ap.x + m_size.x - cs.x,
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              ap.x + m_size.x - m_content.size.x,
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
          case Alignment::Left:
          default:
            tp = olc::vi2d(
              ap.x,
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              ap.x + ts.x,
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
        }
        break;
      case Ordering::ImageFirst:
      default:
        switch (m_content.align) {
          case Alignment::Center:
            tp = olc::vi2d(
              static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f + m_content.size.x),
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f),
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
          case Alignment::Right:
            tp = olc::vi2d(
              ap.x + m_size.x - ts.x,
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              ap.x + m_size.x - cs.x,
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
          case Alignment::Left:
          default:
            tp = olc::vi2d(
              ap.x + m_content.size.x,
              static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f)
            );

            sp = olc::vi2d(
              ap.x,
              static_cast<int>(ap.y + m_size.y / 2.0f - m_content.size.y / 2.0f)
            );
            break;
        }
        break;
    }

    // Draw both the text and the image.
    pge->DrawStringDecal(
      tp,
      m_content.text,
      olc::MAGENTA
    );

    pge->DrawPartialDecal(
      sp,
      m_mcSprite,
      olc::vi2d(),
      ss,
      s
    );
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

    // Compute the size to allocate to each child
    // based on the expanding ones.
    int expandableSize = i;
    int count = m_children.size();
    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      if (!m_children[id]->m_content.expand) {
        expandableSize -= (
          m_layout == Layout::Horizontal ?
          m_children[id]->m_size.x :
          m_children[id]->m_size.y
        );

        log(
          "Child \"" + m_children[id]->getName() + "\" has size " +
          std::to_string(m_layout == Layout::Horizontal ? m_children[id]->m_size.x : m_children[id]->m_size.y) +
          " and expandable is now " + std::to_string(expandableSize)
        );

        --count;
      }

    }

    log("Expandable: " + std::to_string(expandableSize) + ", c: " + std::to_string(count));

    if (expandableSize < 0) {
      log(
        "Menu has " + std::to_string(m_children.size()) + " child(ren)" +
        " occpupying " + std::to_string(i - expandableSize) +
        " but menu is only " + std::to_string(i) +
        ", truncation will occur",
        utils::Level::Warning
      );

      expandableSize = 0;
    }
    count = std::max(count, 1);

    int d = static_cast<int>(1.0f * expandableSize / count);

    // Note that we also force the wraping for the
    // background tiled children to something that
    // will be consistent with their actual size.
    int offset = 0;

    for (unsigned id = 0u ; id < m_children.size() ; ++id) {
      switch (m_layout) {
        case Layout::Vertical:
          m_children[id]->m_size.x = std::min(m_children[id]->m_size.x, wh);
          if (m_children[id]->m_content.expand) {
            m_children[id]->m_size.y = d;
          }

          m_children[id]->m_pos.x = (wh - m_children[id]->m_size.x) / 2.0f;
          m_children[id]->m_pos.y = offset;

          offset += m_children[id]->m_size.y;

          // Update the wrap size for the background of
          // the child menu if it indicates a no-wrap
          // behavior.
          if (!m_children[id]->m_bg.tiling) {
            m_children[id]->m_bg.wrap.x = wh;
            m_children[id]->m_bg.wrap.y = d;
          }

          // We want the icon to occupy all the space
          // available for the child.
          if (m_children[id]->m_content.icon != "") {
            m_children[id]->m_content.size.y = d;
          }
          break;
        case Layout::Horizontal:
        default:
          if (m_children[id]->m_content.expand) {
            m_children[id]->m_size.x = d;
          }
          m_children[id]->m_size.y = std::min(m_children[id]->m_size.y, wh);

          m_children[id]->m_pos.x = offset;
          m_children[id]->m_pos.y = (wh - m_children[id]->m_size.y) / 2.0f;

          offset += m_children[id]->m_size.x;

          if (!m_children[id]->m_bg.tiling) {
            m_children[id]->m_bg.wrap.x = d;
            m_children[id]->m_bg.wrap.y = wh;
          }

          // Note that we cannot force the horizontal
          // size of the icon in this case because we
          // display the text and icon always in the
          // horizontal direction: so if we wanted to
          // set some sort of maximum size it should
          // be consistent with allowing the text to
          // be fully visible or something.
          // If we wanted to have an internal layout
          // (which would indicate whether the icon
          // and text are laid out horizontally or
          // vertically) we could adapt the size of
          // the icon here.
          break;
      }
    }
  }

}

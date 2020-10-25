#ifndef    MENU_CONTENT_DESC_HXX
# define   MENU_CONTENT_DESC_HXX

# include "MenuContentDesc.hh"

namespace new_frontiers {

  inline
  MenuContentDesc
  newTextContent(const std::string& text, const Alignment& align) noexcept {
    return MenuContentDesc{
      text,
      olc::MAGENTA,
      "",
      olc::vi2d(),
      align,
      Ordering::TextFirst,
      true
    };
  }

  inline
  MenuContentDesc
  newImageContent(const std::string& sprite, const olc::vi2d& size) noexcept {
    return MenuContentDesc{
      "",
      olc::MAGENTA,
      sprite,
      size,
      Alignment::Left,
      Ordering::ImageFirst,
      true
    };
  }

  inline
  MenuContentDesc
  newMenuContent(const std::string& text,
                 const std::string& sprite,
                 const olc::vi2d& size,
                 const Alignment& align) noexcept
  {
    return MenuContentDesc{
      text,
      olc::MAGENTA,
      sprite,
      size,
      align,
      Ordering::ImageFirst,
      true
    };
  }

}

#endif    /* MENU_CONTENT_DESC_HXX */

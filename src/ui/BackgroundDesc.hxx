#ifndef    BACKGROUND_DESC_HXX
# define   BACKGROUND_DESC_HXX

# include "BackgroundDesc.hh"

namespace new_frontiers {

  inline
  BackgroundDesc
  newColoredBackground(const olc::Pixel& c) noexcept {
    return BackgroundDesc{c, "", false, olc::vi2d()};
  }

  inline
  BackgroundDesc
  newTiledBackground(const std::string& sprite, const olc::vi2d& wrap) noexcept {
    return BackgroundDesc{olc::Pixel(), sprite, true, wrap};
  }

  inline
  BackgroundDesc
  newImageBackground(const std::string& sprite) noexcept {
    return BackgroundDesc{olc::Pixel(), sprite, false, olc::vi2d()};
  }

}

#endif    /* BACKGROUND_DESC_HXX */

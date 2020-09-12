#ifndef    BACKGROUND_DESC_HXX
# define   BACKGROUND_DESC_HXX

# include "BackgroundDesc.hh"
# include "ColorUtils.hh"

namespace new_frontiers {

  inline
  BackgroundDesc
  newColoredBackground(const olc::Pixel& c) noexcept {
    olc::Pixel hsl = RGBToHSL(c);

    return BackgroundDesc{
      c,
      modulate(c, hsl.b > 127 ? 0.5f : 2.0f),
      "",
      false,
      olc::vi2d()
    };
  }

  inline
  BackgroundDesc
  newTiledBackground(const std::string& sprite, const olc::vi2d& wrap) noexcept {
    return BackgroundDesc{
      olc::WHITE,
      modulate(olc::WHITE, 0.5f),
      sprite,
      true,
      wrap
    };
  }

  inline
  BackgroundDesc
  newImageBackground(const std::string& sprite) noexcept {
    return BackgroundDesc{
      olc::WHITE,
      modulate(olc::WHITE, 0.5f),
      sprite,
      false,
      olc::vi2d()
    };
  }

}

#endif    /* BACKGROUND_DESC_HXX */

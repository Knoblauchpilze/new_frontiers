#ifndef    BACKGROUND_DESC_HH
# define   BACKGROUND_DESC_HH

# include "../olcPixelGameEngine.h"

namespace new_frontiers {

  /**
   * @brief - Convenience structure defining the background that
   *          can be applied to a menu. We either define a color
   *          that is applied on all the menu or a sprite that
   *          is repeated a certain number of times.
   */
  struct BackgroundDesc {
    olc::Pixel color;

    std::string sprite;
    bool wrap;
    olc::vi2d wrap;
  };

  /**
   * @brief - Initializes a background structure defining a color
   *          for the background.
   * @param c - the color to assign to the background.
   * @return - the created background object.
   */
  BackgroundDesc
  newColoredBackground(const olc::Pixel& c) noexcept;

  /**
   * @brief - Create a bew background structure defining a tiled
   *          element with the specified sprite and wrapping.
   * @param sprite - the name of the file representing the sprite.
   * @param wrap - the dimensions of the sprite on screen.
   * @return - the created background object.
   */
  BackgroundDesc
  newTiledBackground(const std::string& sprite, const olc::vi2d& wrap) noexcept;

}

# include "BackgroundDesc.hxx"

#endif    /* BACKGROUND_DESC_HH */

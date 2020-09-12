#ifndef    COLOR_UTILS_HH
# define   COLOR_UTILS_HH

# include "../olcPixelGameEngine.h"

namespace new_frontiers {

  /**
   * @brief - Convert the input color expressed in `RGB`
   *          color space to a color in `HSL` space.
   *          Note that some precision is lost when the
   *          output channels are converted to integer
   *          values.
   * @param rgb - the input color to convert.
   * @return - the equivalent in `HSL` color space.
   */
  olc::Pixel
  RGBToHSL(const olc::Pixel& rgb) noexcept;

  /**
   * @brief - Convert the input color expressed in `HSL`
   *          color space to a color in `RGB` space.
   *          Note that some precision is lost when the
   *          output channels are converted to integer
   *          values.
   * @param hsl - the input color to convert.
   * @return - the equivalent in `RGB` color space.
   */
  olc::Pixel
  HSLToRGB(const olc::Pixel& hsl) noexcept;
}

# include "ColorUtils.hxx"

#endif    /* COLOR_UTILS_HH */

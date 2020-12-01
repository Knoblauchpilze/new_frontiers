#ifndef    COLOR_GENERATOR_HXX
# define   COLOR_GENERATOR_HXX

# include "ColorGenerator.hh"

namespace new_frontiers {

  inline
  olc::Pixel
  ColorGenerator::toColor(unsigned color) noexcept {
    unsigned R = (color / 1000000u) % 1000u;
    unsigned G = (color / 1000u) % 1000u;
    unsigned B = color % 1000u;

    olc::Pixel c;

    c.r = std::min(R, 255u);
    c.g = std::min(G, 255u);
    c.b = std::min(B, 255u);

    return c;
  }

  inline
  unsigned
  ColorGenerator::toUint(unsigned r, unsigned g, unsigned b) noexcept {
    return r * 1000000u + g * 1000u + b;
  }

}

#endif    /* COLOR_GENERARTOR_HXX */

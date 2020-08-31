#ifndef    SOLID_ELEMENT_HXX
# define   SOLID_ELEMENT_HXX

# include "SolidElement.hh"

namespace new_frontiers {

  inline
  SolidElement::SolidElement(const SolidTile& tile,
                             const std::string& name):
    WorldElement<Sprite>(tile, name)
  {}

}

#endif    /* SOLID_ELEMENT_HXX */

#ifndef    SOLID_ELEMENT_HXX
# define   SOLID_ELEMENT_HXX

# include "SolidElement.hh"

namespace new_frontiers {

  inline
  SolidElement::SolidElement(const SolidTile& tile,
                             const std::string& name):
    WorldElement<Sprite>(tile, name)
  {}

  inline
  bool
  SolidElement::step(StepInfo& /*info*/) {
    return false;
  }

  inline
  void
  SolidElement::pause(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  SolidElement::resume(const TimeStamp& /*t*/) {
    // Nothing to do.
  }

}

#endif    /* SOLID_ELEMENT_HXX */

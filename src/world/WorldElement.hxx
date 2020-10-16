#ifndef    WORLD_ELEMENT_HXX
# define   WORLD_ELEMENT_HXX

# include "WorldElement.hh"

namespace new_frontiers {

  inline
  bool
  WorldElement::isOwned() const noexcept {
    return m_owner.valid();
  }

  inline
  const utils::Uuid&
  WorldElement::getOwner() const noexcept {
    return m_owner;
  }

  inline
  WorldElement::WorldElement(const std::string& name,
                             const utils::Uuid& owner):
    utils::CoreObject(name),

    m_owner(owner)
  {
  }

  inline
  void
  WorldElement::setOwner(const utils::Uuid& uuid) {
    m_owner = uuid;
  }

}

#endif    /* WORLD_ELEMENT_HXX */

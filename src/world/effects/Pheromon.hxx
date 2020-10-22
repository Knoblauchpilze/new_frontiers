#ifndef    PHEROMON_HXX
# define   PHEROMON_HXX

# include "Pheromon.hh"

namespace new_frontiers {

  inline
  Pheromon::Pheromon(const PProps& props):
    EvaporatingVFX(props),

    m_type(props.type)
  {}

  inline
  const pheromon::Type&
  Pheromon::getType() const noexcept {
    return m_type;
  }

}

#endif    /* PHEROMON_HXX */

#ifndef    PHEROMON_HXX
# define   PHEROMON_HXX

# include "Pheromon.hh"

namespace new_frontiers {

  inline
  Pheromon::Pheromon(const pheromon::Type& type,
                     const VFXTile& vfx,
                     float evaporation):
    EvaporatingVFX(vfx, evaporation),

    m_type(type)
  {}

}

#endif    /* PHEROMON_HXX */

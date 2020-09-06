#ifndef    PHEROMON_HXX
# define   PHEROMON_HXX

# include "Pheromon.hh"

namespace new_frontiers {

  inline
  Pheromon::Pheromon(const pheromon::Type& type,
                     const VFXTile& vfx,
                     const std::vector<Duration>& decay):
    VFX(vfx, decay),
    
    m_type(type)
  {}

}

#endif    /* PHEROMON_HXX */

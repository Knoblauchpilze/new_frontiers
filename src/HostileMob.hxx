#ifndef    HOSTILE_MOB_HXX
# define   HOSTILE_MOB_HXX

# include "HostileMob.hh"

namespace new_frontiers {

  inline
  HostileMob::HostileMob(const EntityTile& tile):
    Entity(tile, Smoke, 2, false),

    m_vfxDelay(toMilliseconds(6000)),
    m_last(now())
  {}

}

#endif    /* HOSTILE_MOB_HXX */

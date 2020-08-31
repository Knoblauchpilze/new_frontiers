#ifndef    ENTITY_HXX
# define   ENTITY_HXX

# include "Entity.hh"

namespace new_frontiers {

  inline
  void
  PathSegment::animate(const TimeStamp& moment, float& x, float& y) const {
    // Compute the ratio between this moment and
    // the total time and use this as a percentage
    // of completion for the path.
    float p = 1.0f;

    if (start != end) {
      Duration d = moment - start;
      Duration t = end - start;

      p = std::min(std::max(1.0f * d.count() / t.count(), 0.0f), 1.0f);
    }

    x = (1.0f - p) * xO + p * xT;
    y = (1.0f - p) * yO + p * yT;
  }

  inline
  Entity::Entity(const EntityTile& desc,
                 const Effect& vfx):
    WorldElement(desc, "entity"),

    m_vfx(vfx),

    m_speed(-1.0f),
    m_rArrival(0.2f),
    m_pathLength(3.0f),

    m_hasPath(false),
    m_path()
  {}

}

#endif    /* ENTITY_HXX */

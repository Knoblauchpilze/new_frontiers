
# include "CoordinateFrames.hh"

namespace new_frontiers {

  CoordinateFrames::CoordinateFrames(const olc::vi2d& worldOrigin,
                                     const olc::vi2d& sprite,
                                     const olc::vi2d& tile):
    m_wo(worldOrigin),

    m_ss(sprite),
    m_ts(tile),

    m_translationOrigin(),
    m_cachedWo()
  {}

  void
  CoordinateFrames::translate(const olc::vi2d& pos) {
    // We need to deduce the translation added by
    // the input `pos` assuming that this will be
    // the final position of the world's origin.
    olc::vi2d translation = pos - m_translationOrigin;
    m_wo = m_cachedWo + translation;
  }

}

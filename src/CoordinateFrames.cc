
# include "CoordinateFrames.hh"

namespace new_frontiers {

  CoordinateFrames::CoordinateFrames(const Viewport& cViewport,
                                     const Viewport& pViewport,
                                     const olc::vi2d& tileSize):
    utils::CoreObject("frame"),

    m_cViewport(cViewport),
    m_pViewport(pViewport),

    m_ts(tileSize),
    m_scale(1.0f, 1.0f),
    m_tScaled(m_ts),

    m_translationOrigin(),
    m_cachedPOrigin()
  {
    setService("coordinate");

    // Update the tiles scale based on the viewport.
    updateTileScale();
  }

  void
  CoordinateFrames::updateTileScale() {
    m_tScaled = m_pViewport.dims / m_cViewport.dims;
    m_scale = m_tScaled / m_ts;
  }

}

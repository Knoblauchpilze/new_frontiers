
# include "CoordinateFrames.hh"
# include "utils.hh"

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

    log("cViewport: " + toString(m_cViewport.dims), utils::Level::Info);
    log("pViewport: " + toString(m_pViewport.dims), utils::Level::Info);
    log("real size: " + toString(m_ts), utils::Level::Info);
    log("on screen: " + toString(m_scale), utils::Level::Info);
    log("scaling  : " + toString(m_tScaled), utils::Level::Info);
  }

  void
  CoordinateFrames::updateTileScale() {
    m_tScaled = m_pViewport.dims / m_cViewport.dims;
    m_scale = m_tScaled / m_ts;
  }

}

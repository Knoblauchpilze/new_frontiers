
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
    m_tScaled.x = 1.0f * m_pViewport.dims.x / m_cViewport.dims.x;
    m_tScaled.y = 1.0f * m_pViewport.dims.y / m_cViewport.dims.y;

    m_scale = m_tScaled / m_ts;

    log("cViewport: " + toString(m_cViewport.dims), utils::Level::Info);
    log("pViewport: " + toString(m_pViewport.dims), utils::Level::Info);
    log("real size: " + toString(m_ts), utils::Level::Info);
    log("on screen: " + toString(m_scale), utils::Level::Info);
    log("scaling  : " + toString(m_tScaled), utils::Level::Info);
  }

}

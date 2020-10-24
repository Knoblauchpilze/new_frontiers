
# include "PheromonAnalyzer.hh"
# include <algorithm>

namespace new_frontiers {

  PheromonAnalyzer::PheromonAnalyzer():
    utils::CoreObject("analyze"),

    m_weights(),
    m_rndWeight(),

    m_info()
  {
    setService("pheromons");

    initialize();
  }

  void
  PheromonAnalyzer::computeTarget(float& x, float& y) {
    // Make a copy of the pheromons accumulated so far.
    // This will allow to keep it intact in case more
    // than a single target needs to be computed.
    std::unordered_map<pheromon::Type, PheromonInfo> pheromons = m_info;

    // Compute whether or not some pheromons have been
    // accumulated so far. We will also report info on
    // the relative weight of the pheromon directly
    // from the `m_weights` table.
    // In case no information is provided in the table
    // we will assign a weight of `0`.
    int count = 0;
    for (InfoIt it = pheromons.begin() ; it != pheromons.end() ; ++it) {
      // Normalize (i.e. take into account the amount
      // of pheromons available).
      it->second.normalize();

      // Report the relative weight.
      CWIt wit = m_weights.find(it->first);

      // In case the pheromon is not registered we
      // won't consider that it is relevant for the
      // `count`.
      if (wit == m_weights.cend()) {
        continue;
      }

      // Register the relative weight and the count.
      it->second.w = wit->second;
      count += it->second.count;
    }

    if (count == 0) {
      // Use only random, nothing to do.
      return;
    }

    // Adjust weights based on whether or not some
    // pheromons are visible: this will guarantee
    // that the relative weight for pheromons are
    // respected even when some are not existing.
    float wRnd = m_rndWeight;
    float xRnd = x, yRnd = y;

    for (InfoIt it = pheromons.begin() ; it != pheromons.end() ; ++it) {
      // In case there are no accumulated pheromons for
      // this type, reevaluate the weights for others
      // so that we still get something consistent with
      // the initial distribution.
      // Note that we will only handle this process for
      // pheromons that have a weight defined.
      if (it->second.count == 0) {
        float s = 1.0f - it->second.w;

        std::for_each(
          pheromons.begin(),
          pheromons.end(),
          [&it, &s](PIKey& k) {
            if (k.first != it->first) {
              k.second.w /= s;
            }
          }
        );

        it->second.w = 0.0f;
      }
    }

    // Pick a random target.
    float totW = std::accumulate(
      pheromons.cbegin(),
      pheromons.cend(),
      wRnd,
      [](float w, const PIKey& k) {
        return w + k.second.w;
      }
    );

    x = std::accumulate(
      pheromons.cbegin(),
      pheromons.cend(),
      0.0f,
      [](float x, const PIKey& p) {
        return x + p.second.x * p.second.w;
      }
    );
    x += (xRnd * wRnd);

    y = std::accumulate(
      pheromons.cbegin(),
      pheromons.cend(),
      0.0f,
      [](float y, const PIKey& p) {
        return y + p.second.y * p.second.w;
      }
    );
    y += (yRnd * wRnd);

    x /= totW;
    y /= totW;

    log(
      "rn: " + std::to_string(xRnd) + "x" +  std::to_string(yRnd) +
      ", ch: " + std::to_string(pheromons[pheromon::Type::Chase].x) + "x" +
      std::to_string(pheromons[pheromon::Type::Chase].y) +
      ", fi: " + std::to_string(pheromons[pheromon::Type::Fight].x) + "x" +
      std::to_string(pheromons[pheromon::Type::Fight].y) +
      ", co: " + std::to_string(pheromons[pheromon::Type::Collect].x) + "x" +
      std::to_string(pheromons[pheromon::Type::Collect].y) +
      ", re: " + std::to_string(pheromons[pheromon::Type::Return].x) + "x" +
      std::to_string(pheromons[pheromon::Type::Return].y) +
      ", wa: " + std::to_string(pheromons[pheromon::Type::Wander].x) + "x" +
      std::to_string(pheromons[pheromon::Type::Wander].y) +
      ", fi: " + std::to_string(x) + "x" + std::to_string(y) +
      " (weights: " + std::to_string(wRnd) +
      ", " + std::to_string(pheromons[pheromon::Type::Chase].w) +
      ", " + std::to_string(pheromons[pheromon::Type::Fight].w) +
      ", " + std::to_string(pheromons[pheromon::Type::Collect].w) +
      ", " + std::to_string(pheromons[pheromon::Type::Return].w) +
      ", " + std::to_string(pheromons[pheromon::Type::Wander].w) +
      ", total: " + std::to_string(totW) + ")" +
      " (counts: " + std::to_string(pheromons[pheromon::Type::Chase].count) +
      ", " + std::to_string(pheromons[pheromon::Type::Fight].count) +
      ", " + std::to_string(pheromons[pheromon::Type::Collect].count) +
      ", " + std::to_string(pheromons[pheromon::Type::Return].count) +
      ", " + std::to_string(pheromons[pheromon::Type::Wander].count) + ")",
      utils::Level::Debug
    );
  }

}

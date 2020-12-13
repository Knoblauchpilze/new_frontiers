#ifndef    PHEROMON_ANALYZER_HXX
# define   PHEROMON_ANALYZER_HXX

# include "PheromonAnalyzer.hh"

namespace new_frontiers {

  inline
  void
  PheromonAnalyzer::PheromonInfo::accumulate(float locX, float locY, float amount) {
    x += amount * locX;
    y += amount * locY;
    w += amount;

    ++count;
  }

  inline
  void
  PheromonAnalyzer::PheromonInfo::normalize() {
    if (count > 0) {
      x /= w;
      y /= w;

      w = 1.0f;
    }
  }

  inline
  void
  PheromonAnalyzer::setRelativeWeight(const pheromon::Type& type, float w) {
    m_weights[type] = w;
  }

  inline
  void
  PheromonAnalyzer::setRandomWeight(float w) {
    m_rndWeight = w;
  }

  inline
  void
  PheromonAnalyzer::accumulate(const Pheromon& p) {
    // Fetch the pheromon in the internally accumulated
    // information table.
    InfoIt it = m_info.find(p.getType());

    if (it == m_info.end()) {
      // Just register a new information, but this should
      // not really happen.
      m_info[p.getType()] = PheromonInfo{p.getTile().p.x(), p.getTile().p.y(), p.getAmount(), 1};

      return;
    }

    // Accumulate the information.
    it->second.accumulate(p.getTile().p.x(), p.getTile().p.y(), p.getAmount());
  }

  inline
  void
  PheromonAnalyzer::initialize() {
    m_weights[pheromon::Type::Chase] = 0.05f;
    m_weights[pheromon::Type::Fight] = 0.3f;
    m_weights[pheromon::Type::Collect] = 0.15f;
    m_weights[pheromon::Type::Return] = 0.15f;
    m_weights[pheromon::Type::Wander] = 0.0f;
    m_weights[pheromon::Type::Flee] = 0.05f;

    m_rndWeight = 0.3f;

    // Register default quantity for each pheromon.
    PheromonInfo da{0.0f, 0.0f, 0.0f, 0};

    m_info[pheromon::Type::Chase] = da;
    m_info[pheromon::Type::Fight] = da;
    m_info[pheromon::Type::Collect] = da;
    m_info[pheromon::Type::Return] = da;
    m_info[pheromon::Type::Wander] = da;
    m_info[pheromon::Type::Flee] = da;
  }

}

#endif    /* PHEROMON_ANALYZER_HXX */


# include "ColorGenerator.hh"

namespace new_frontiers {

  ColorGenerator::ColorGenerator(unsigned tries,
                                 bool preventBlack):
    m_tries(tries),
    m_preventBlack(preventBlack),

    m_colors(),
    m_existing()
  {}

  olc::Pixel
  ColorGenerator::generate(const std::string& key, bool& failed) {
    // Check whether this key exists.
    Colors::const_iterator c = m_colors.find(key);
    if (c != m_colors.cend()) {
      return toColor(c->second);
    }

    // The key has not been encountered yet:
    // generate a new color.
    unsigned tries = 0u;
    unsigned color = 0u;
    bool valid = false;

    unsigned offset = (m_preventBlack ? 1u : 0u);
    unsigned range = (m_preventBlack ? 255u : 256u);

    unsigned r = 0u, g = 0u, b = 0u;

    while (!valid && tries < m_tries) {
      r = offset + rand() % range;
      g = offset + rand() % range;
      b = offset + rand() % range;

      color = toUint(r, g, b);

      Cache::const_iterator exist = m_existing.find(color);
      valid = (exist == m_existing.cend());

      ++tries;
    }

    failed = !valid;
    if (failed) {
      return olc::BLACK;
    }

    m_colors[key] = color;
    m_existing.insert(color);

    return toColor(color);
  }

}


# include "RNG.hh"

namespace new_frontiers {

  RNG::RNG(int seed):
    utils::CoreObject("rng"),

    m_rng(seed)
  {}

}

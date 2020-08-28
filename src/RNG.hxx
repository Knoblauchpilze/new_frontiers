#ifndef    RNG_HXX
# define   RNG_HXX

# include "RNG.hh"

namespace new_frontiers {

  inline
  RNG
  RNG::createRandomRNG() noexcept {
    // Create a random device. This will be used to
    // provide the seed of the RNG. This articla is
    // nice to explain how things work here:
    // https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c
    std::random_device device;

    // Use this as a seed.
    return RNG(device());
  }

  inline
  int
  RNG::rndInt(int min, int max) noexcept {
    // Generate an integer distribution and use it.
    std::uniform_int_distribution<int> dist(min, max);
    return dist(m_rng);
  }

  inline
  float
  RNG::rndFloat(float min, float max) noexcept {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_rng);
  }

}

#endif    /* RNG_HXX */

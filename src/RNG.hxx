#ifndef    RNG_HXX
# define   RNG_HXX

# include "RNG.hh"

namespace new_frontiers {

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

}

#endif    /* RNG_HXX */

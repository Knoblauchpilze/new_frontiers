#ifndef    RNG_HH
# define   RNG_HH

# include <core_utils/CoreObject.hh>
# include <random>

namespace new_frontiers {

  class RNG: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new RNG with the specified seed. This
       *          is a convenient way to ensure that all needs
       *          for a random number are satisfied by a single
       *          class and thus ensure reproctability of runs
       *          within the application.
       * @param seed - the seed to use for this RNG.
       */
      RNG(int seed = 0);

      /**
       * @brief - Used to perform the creation of a random number
       *          generator initialized with some seed not known
       *          to the user. Compared to the canonical method
       *          above it avoid the user to care about generating
       *          a seed.
       * @return - the created RNG.
       */
      static
      RNG
      createRandomRNG() noexcept;

    private:

      /**
       * @brief - The actual RNG. It produces high quality random
       *          numbers in the range `[0; 2^32 - 1]` as defined
       *          in the CPP reference.
       */
      std::mt19937 m_rng;
  };

}

# include "RNG.hxx"

#endif    /* RNG_HH */

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

      /**
       * @brief - Used to generate a random number in the range
       *          defined by the input value.
       * @param min - the minimum value to generate.
       * @param max - the maximum value allowed to be generated.
       * @return - a random value in the range `[min; max]`.
       */
      int
      rndInt(int min, int max) noexcept;

      /**
       * @brief - Used to generate a random number in the range
       *          defined by the input value. Similar to the
       *          `rndInt` method but for floats.
       * @param min - the minimum value to generate.
       * @param max - the maximum value allowed to be generated.
       * @return - a random value in the range `[min; max]`.
       */
      float
      rndFloat(float min, float max) noexcept;

      /**
       * @brief - Used as a convenience method to generate a
       *          random angle in the range `[min; max]`.
       * @param min - the min angle that can be generated.
       *              Default value is `0`.
       * @param max - the max angle that can be generated.
       *              Default value is `2pi`.
       * @return - an value in the range `[min; max]`.
       */
      float
      rndAngle(float min = 0.0f, float max = 6.283185307f) noexcept;

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

#ifndef    VFX_HH
# define   VFX_HH

# include <memory>
# include "WorldElement.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  class VFX: public WorldElement<Effect> {
    public:

      /**
       * @brief - Create a new VFX with the specified tile.
       * @param tile - the visual display for this VFX and
       *               its associated position.
       */
      VFX(const VFXTile& tile);

      /**
       * @brief - Implementation of the interface method to
       *          make this visual effect evolve.
       * @param info - the necessary information to describe
       *               the world's surroundings.
       * @return - `true` if this effect should be deleted.
       */
      virtual bool
      step(StepInfo& info);

    private:

      /**
       * @brief - Defines the duration of the decay time for
       *          a single transition for this effect. It is
       *          expressed in milliseconds.
       */
      Duration m_decay;

      /**
       * @brief - Custom duration for the last transition. We
       *          usually want to make it a bit longer.
       */
      Duration m_lastDecay;

      /**
       * @brief - The numebr of transitions that this effect
       *          can go through before finally disappearing.
       */
      int m_transitions;

      /**
       * @brief - The next time at which the effect will decay.
       *          It is updated when this point in time is
       *          reached in the simulation.
       */
      TimeStamp m_phase;
  };

  using VFXShPtr = std::shared_ptr<VFX>;
}

# include "VFX.hxx"

#endif    /* VFX_HH */

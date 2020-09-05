#ifndef    VFX_HH
# define   VFX_HH

# include <memory>
# include "Element.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  class VFX: public Element<Effect> {
    public:

      /**
       * @brief - Create a new VFX with the specified tile.
       * @param tile - the visual display for this VFX and
       *               its associated position.
       * @param decaying - `true` if this VFX is decaying.
       */
      VFX(const VFXTile& tile, bool decaying);

      /**
       * @brief - Implementation of the interface method to
       *          make this visual effect evolve.
       * @param info - the necessary information to describe
       *               the world's surroundings.
       * @return - `true` if this effect should be deleted.
       */
      bool
      step(StepInfo& info) override;

      /**
       * @brief - Implementation of the interface method to
       *          pause the internal processes for this VFX.
       *          It mostly involves pausing the decaying if
       *          it is relevant.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to
       *          resume the internal processes for this VFX.
       *          We will resume the decaying operations if
       *          need be.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    private:

      /**
       * @brief - If `true`, the vfx will use a certain
       *          number of transitions before decaying.
       *          If `false` only the `m_lastDecay` time
       *          will be used and then the VFX will be
       *          removed.
       */
      bool m_decaying;

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

      /**
       * @brief - This duration is used in the case of a `pause`
       *          operation to save the time that was left on
       *          the current decay operation. It will be used
       *          to restore the same amount of decaying time
       *          when the effect is resumed.
       */
      Duration m_decayTimeLeft;
  };

  using VFXShPtr = std::shared_ptr<VFX>;
}

# include "VFX.hxx"

#endif    /* VFX_HH */

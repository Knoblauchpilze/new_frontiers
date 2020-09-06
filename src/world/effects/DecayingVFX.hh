#ifndef    DECAYING_VFX_HH
# define   DECAYING_VFX_HH

# include <memory>
# include "VFX.hh"

namespace new_frontiers {

  class DecayingVFX: public VFX {
    public:

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and decay times.
       * @param tile - the visual display for this effect which
       *               also contain the position at which the
       *               effect should be spawned.
       * @param phases - the list of phases that this effect has
       *                 to traverse before finally decaying.
       */
      DecayingVFX(const VFXTile& tile, const std::vector<Duration>& phases);

      void
      pause(const TimeStamp& t) override;

      void
      resume(const TimeStamp& t) override;

    protected:

      /**
       * @brief - Implementation of the interface method allowing
       *          tp detect whether this effect is terminated. We
       *          will check whether all transitions successfully
       *          terminated to provide an answer.
       * @param moment - the moment at which the effect should be
       *                 checked for termination.
       * @param return - `true` if the effect should be marked for
       *                 removal.
       */
      bool
      isTerminated(const TimeStamp& moment) const noexcept override;

      /**
       * @brief - Implementation of the interface method to make
       *          this effect evolve. We will proceed to checking
       *          whether a decay should occur and move on to the
       *          next transition for this effect.
       * @param step - information about the current state of the
       *               simulation.
       */
      void
      update(StepInfo& step) noexcept override;

    private:

      /**
       * @brief - Holds the list of phases for this effect and
       *          their associated durations. Each phase will
       *          be traversed successively starting from the
       *          index `0`.
       *          In case no phase is provided, the effect is
       *          kept as it is forever.
       */
      std::vector<Duration> m_phases;

      /**
       * @brief - The index of the next transition that should
       *          be used by this effect. Once the last index
       *          is reached in the `m_phases` array the effect
       *          is scheduled for deletion.
       */
      unsigned m_transition;

      /**
       * @brief - The next time at which the effect will decay.
       *          It is updated when this point in time is
       *          reached in the simulation.
       */
      TimeStamp m_next;

      /**
       * @brief - This duration is used in the case of a `pause`
       *          operation to save the time that was left on
       *          the current decay operation. It will be used
       *          to restore the same amount of decaying time
       *          when the effect is resumed.
       */
      Duration m_decayTimeLeft;
  };

}

# include "DecayingVFX.hxx"

#endif    /* DECAYING_VFX_HH */

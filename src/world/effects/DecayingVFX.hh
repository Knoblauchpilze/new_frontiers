#ifndef    DECAYING_VFX_HH
# define   DECAYING_VFX_HH

# include <memory>
# include "VFX.hh"

namespace new_frontiers {

  class DecayingVFX: public VFX {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create an VFX. It is meant as a
       *          way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct DProps: Props {
        std::vector<utils::Duration> phases;
      };

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and decay times.
       * @param props - the description of this decaying VFX.
       */
      DecayingVFX(const DProps& props);

      void
      pause(const utils::TimeStamp& t) override;

      void
      resume(const utils::TimeStamp& t) override;

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
      isTerminated(const utils::TimeStamp& moment) const noexcept override;

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
      std::vector<utils::Duration> m_phases;

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
      utils::TimeStamp m_next;

      /**
       * @brief - This duration is used in the case of a `pause`
       *          operation to save the time that was left on
       *          the current decay operation. It will be used
       *          to restore the same amount of decaying time
       *          when the effect is resumed.
       */
      utils::Duration m_decayTimeLeft;
  };

}

# include "DecayingVFX.hxx"

#endif    /* DECAYING_VFX_HH */

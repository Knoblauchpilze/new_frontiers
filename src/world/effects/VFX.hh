#ifndef    VFX_HH
# define   VFX_HH

# include <memory>
# include "Element.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  class VFX: public Element<tiles::Effect> {
    public:

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

    protected:

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and decay times.
       * @param tile - the visual display for this effect which
       *               also contain the position at which the
       *               effect should be spawned.
       * @param phases - the list of phases that this effect has
       *                 to traverse before finally decaying.
       */
      VFX(const VFXTile& tile, const std::vector<Duration>& phases);

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

  using VFXShPtr = std::shared_ptr<VFX>;
}

# include "VFX.hxx"

#endif    /* VFX_HH */

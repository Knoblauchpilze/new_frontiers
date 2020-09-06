#ifndef    VFX_HH
# define   VFX_HH

# include <memory>
# include "Element.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  class VFX: public Element<tiles::Effect> {
    public:

      /**
       * @brief - Return the amount of the effect that still
       *          exist compared to the initial quantity. It
       *          is a rather abstract concept at this level
       *          but it allows to get an idea of how the
       *          effect has dissipated since its creation.
       * @return - the amount of the initial quantity of the
       *           effect that still exists.
       */
      float
      getAmount() const noexcept;

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
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to pause.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to resume.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and name.
       * @param tile - the visual display for this effect which
       *               also contain the position at which the
       *               effect should be spawned.
       * @param name - the name of this VFX in logs.
       */
      VFX(const VFXTile& tile, const std::string& name);

      /**
       * @brief - Interface method defining whether or not the
       *          effect should be terminated. It is used here
       *          upon processing the `step` function to check
       *          whether the effect should be removed by the
       *          simulation or not. Depending on the actual
       *          type of the effect, the implementation of
       *          this method might differ.
       *          This class provide the moment at which the
       *          answer is requested: it allows inheriting
       *          classes to to time-base removal.
       * @param moment - the moment at which the effect should
       *                 be checked for termination.
       * @param return - `true` if the effect should be marked
       *                 for removal.
       */
      virtual bool
      isTerminated(const TimeStamp& moment) const noexcept = 0;

      /**
       * @brief - Interface method allowing to make an effect
       *          update itself based on its environment. This
       *          is an internal version of the `step` method
       *          to ensure that some processes are handled no
       *          matter the actual implementation of the VFX.
       * @param step - information about the current state of
       *               the simulation.
       */
      virtual void
      update(StepInfo& step) noexcept = 0;

      /**
       * @brief - Used to change the amount of the effect that
       *          exists by making it vary of `delta` unit. It
       *          can either be a positive or negative value
       *          the only guarantee being that the final value
       *          will be clamped to be in the range `[0; 1]`.
       * @param delta - the change in the amount.
       */
      void
      changeAmount(float delta) noexcept;

    private:

      /**
       * @brief - Define the amount of the effect that remains
       *          at any given moment assuming the initial value
       *          was `1`.
       *          At this level it is rather abstract but it can
       *          be used to measure how the initial effect that
       *          was produced by this object is continuing.
       *          It can be used for example to adjust the level
       *          of damage that an entity takes if this effect
       *          is a poisonous gas, or the amount of desire a
       *          pheromon still has compared to its initial
       *          value.
       *          This vauls is in the range `[0; 1]`.
       */
      float m_amount;
  };

  using VFXShPtr = std::shared_ptr<VFX>;
}

# include "VFX.hxx"

#endif    /* VFX_HH */

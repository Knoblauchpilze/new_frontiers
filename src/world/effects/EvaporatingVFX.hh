#ifndef    EVAPORATING_VFX_HH
# define   EVAPORATING_VFX_HH

# include <memory>
# include "VFX.hh"

namespace new_frontiers {

  class EvaporatingVFX: public VFX {
    public:

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and decay times.
       * @param tile - the visual display for this effect which
       *               also contain the position at which the
       *               effect should be spawned.
       * @param radius - the radius for this VFX.
       * @param evaporation - defines the evaporation rate for
       *                      this effect in unit per second.
       */
      EvaporatingVFX(const VFXTile& tile,
                     float radius,
                     float evaporation);

    protected:

      bool
      isTerminated(const TimeStamp& moment) const noexcept override;

      void
      update(StepInfo& step) noexcept override;

    private:

      /**
       * @brief - Define the rat at which this effect evaporates.
       *          The larger this value the quicker the effect is
       *          to disappear.
       *          This value is expressed in units per second. If
       *          the speed is `0` the effect won't evaporate. A
       *          negative value is clamped to `0` upon building
       *          the object.
       */
      float m_evaporation;
  };

}

# include "EvaporatingVFX.hxx"

#endif    /* EVAPORATING_VFX_HH */

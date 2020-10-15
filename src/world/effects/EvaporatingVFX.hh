#ifndef    EVAPORATING_VFX_HH
# define   EVAPORATING_VFX_HH

# include <memory>
# include "VFX.hh"

namespace new_frontiers {

  class EvaporatingVFX: public VFX {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create an VFX. It is meant as a
       *          way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct EProps: Props {
        float evaporation;
      };

      /**
       * @brief - Create a new effect with the specified visual
       *          representation and decay times.
       * @param props - the description of the props of this VFX.
       */
      EvaporatingVFX(const EProps& props);

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

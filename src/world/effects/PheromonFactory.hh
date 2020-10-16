#ifndef    PHEROMON_FACTORY_HH
# define   PHEROMON_FACTORY_HH

# include "DecayingVFX.hh"
# include "EvaporatingVFX.hh"
# include "Pheromon.hh"

namespace new_frontiers {

  class PheromonFactory {
    public:

      static
      DecayingVFX::DProps
      newDecayingProps(float x, float y, const tiles::Effect& vfx) noexcept;

      static
      EvaporatingVFX::EProps
      newEvaporatingProps(float x, float y, const tiles::Effect& vfx) noexcept;

      static
      Pheromon::PProps
      newPheromonProps(float x, float y, const tiles::Effect& vfx) noexcept;

      static
      tiles::Effect
      pheromonToVFX(const pheromon::Type& type) noexcept;

      static
      PheromonShPtr
      newPheromon(const Pheromon::PProps& props) noexcept;

    private:

      static
      VFXTile
      newTile(const tiles::Effect& e, int id, float x, float y) noexcept;

    private:

      static const float sk_radius;
  };

}

# include "PheromonFactory.hxx"

#endif    /* PHEROMON_FACTORY_HH */

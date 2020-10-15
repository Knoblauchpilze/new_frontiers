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
      PheromonShPtr
      newPheromon(const pheromon::Type& type, float x, float y, float radius) noexcept;

    private:

      static
      VFXTile
      newTile(const tiles::Effect& e, int id, float x, float y) noexcept;

      static
      tiles::Effect
      pheromonToVFX(const pheromon::Type& type) noexcept;

    private:

      static const float sk_radius;
  };

}

# include "PheromonFactory.hxx"

#endif    /* PHEROMON_FACTORY_HH */

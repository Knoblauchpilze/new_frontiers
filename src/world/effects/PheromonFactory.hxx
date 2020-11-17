#ifndef    PHEROMON_FACTORY_HXX
# define   PHEROMON_FACTORY_HXX

# include "PheromonFactory.hh"

namespace new_frontiers {

  inline
  DecayingVFX::DProps
  PheromonFactory::newDecayingProps(float x, float y, const tiles::Effect& vfx) noexcept {
    DecayingVFX::DProps pp;

    pp.tile = newTile(vfx, 0, x, y);
    pp.radius = sk_radius;

    pp.amount = 1.0f;

    pp.owner.invalidate();

    static const int b = 1000;
    std::vector<Duration> decay = {
      toMilliseconds(b),
      toMilliseconds(2 * b),
      toMilliseconds(4 * b),
      toMilliseconds(8 * b),
      toMilliseconds(8 * b)
    };

    pp.phases.swap(decay);

    return pp;
  }

  inline
  EvaporatingVFX::EProps
  PheromonFactory::newEvaporatingProps(float x, float y, const tiles::Effect& vfx) noexcept {
    EvaporatingVFX::EProps pp;

    pp.tile = newTile(vfx, 0, x, y);
    pp.radius = sk_radius;

    pp.amount = 1.0f;

    pp.owner.invalidate();

    pp.evaporation = 0.1f;

    return pp;
  }

  inline
  Pheromon::PProps
  PheromonFactory::newPheromonProps(float x, float y, const pheromon::Type& type) noexcept {
    Pheromon::PProps pp;

    pp.tile = newTile(pheromonToVFX(type), 0, x, y);
    pp.radius = sk_radius;

    pp.amount = 1.0f;

    pp.owner.invalidate();

    pp.evaporation = 0.02f;

    pp.type = type;

    return pp;
  }

  inline
  tiles::Effect
  PheromonFactory::pheromonToVFX(const pheromon::Type& type) noexcept {
    switch (type) {
      case pheromon::Type::Collect:
        return tiles::Fire;
      case pheromon::Type::Chase:
      case pheromon::Type::Fight:
        return tiles::Poison;
      case pheromon::Type::Return:
        return tiles::Lightning;
      case pheromon::Type::Flee:
      case pheromon::Type::Wander:
      default:
        return tiles::Smoke;
    }
  }

  inline
  PheromonShPtr
  PheromonFactory::newPheromon(const Pheromon::PProps& props) noexcept {
    // Evaporation time is fixed for now.
    return std::make_shared<Pheromon>(props);
  }

  inline
  VFXTile
  PheromonFactory::newTile(const tiles::Effect& e, int id, float x, float y) noexcept {
    return VFXTile{x, y, e, id};
  }

}

#endif    /* PHEROMON_FACTORY_HXX */

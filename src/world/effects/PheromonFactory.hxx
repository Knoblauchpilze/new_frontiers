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

    // TODO: Handle this ?
    // pp.phases;

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
  PheromonFactory::newPheromonProps(float x, float y, const tiles::Effect& vfx) noexcept {
    Pheromon::PProps pp;

    pp.tile = newTile(vfx, 0, x, y);
    pp.radius = sk_radius;

    pp.amount = 1.0f;

    pp.owner.invalidate();

    pp.evaporation = 0.1f;

    pp.type = pheromon::Type::Wander;

    return pp;
  }

  inline
  PheromonShPtr
  PheromonFactory::newPheromon(const pheromon::Type& type, float x, float y, float radius) noexcept {
    // Deduce the VFX type from the pheromon type.
    tiles::Effect vt = pheromonToVFX(type);

    Pheromon::PProps pp = newPheromonProps(x, y, vt);
    pp.radius = radius;

    pp.type = type;

    // Evaporation time is fixed for now.
    return std::shared_ptr<Pheromon>(new Pheromon(pp));
  }

  inline
  VFXTile
  PheromonFactory::newTile(const tiles::Effect& e, int id, float x, float y) noexcept {
    return VFXTile{x, y, e, id};
  }

  inline
  tiles::Effect
  PheromonFactory::pheromonToVFX(const pheromon::Type& type) noexcept {
    switch (type) {
      case pheromon::Type::Chase:
      case pheromon::Type::Collect:
        return tiles::Fire;
      case pheromon::Type::Fight:
        return tiles::Poison;
      case pheromon::Type::Return:
        return tiles::Lightning;
      case pheromon::Type::Wander:
      default:
        return tiles::Smoke;
    }
  }

}

#endif    /* PHEROMON_FACTORY_HXX */

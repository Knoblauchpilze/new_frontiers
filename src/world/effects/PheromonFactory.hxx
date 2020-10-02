#ifndef    PHEROMON_FACTORY_HXX
# define   PHEROMON_FACTORY_HXX

# include "PheromonFactory.hh"

namespace new_frontiers {

  inline
  PheromonShPtr
  PheromonFactory::newPheromon(const pheromon::Type& type, float x, float y, float radius) noexcept {
    // Deduce the VFX type from the pheromon type.
    tiles::Effect vt = pheromonToVFX(type);
    VFXTile vfx = newTile(vt);
    vfx.x = x; vfx.y = y;

    // Evaporation time is fixed for now.
    return std::shared_ptr<Pheromon>(new Pheromon(type, vfx, radius, 0.1f));
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

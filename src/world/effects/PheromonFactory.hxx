#ifndef    PHEROMON_FACTORY_HXX
# define   PHEROMON_FACTORY_HXX

# include "PheromonFactory.hh"

namespace new_frontiers {

  inline
  PheromonShPtr
  PheromonFactory::newPheromon(const pheromon::Type& type, float x, float y) noexcept {
    // Deduce the VFX type from the pheromon type.
    tiles::Effect vt = pheromonToVFX(type);
    VFXTile vfx = newTile(vt);
    vfx.x = x; vfx.y = y;

    // Build the VFX knowing that the decay table
    // for pheromon is hard-coded for now.
    std::vector<Duration> decay;

    static const int b = 1000;
    decay.push_back(toMilliseconds(b));
    decay.push_back(toMilliseconds(2 * b));
    decay.push_back(toMilliseconds(4 * b));
    decay.push_back(toMilliseconds(8 * b));
    decay.push_back(toMilliseconds(8 * b));

    return std::shared_ptr<Pheromon>(new Pheromon(type, vfx, decay));
  }

  inline
  tiles::Effect
  PheromonFactory::pheromonToVFX(const pheromon::Type& type) noexcept {
    switch (type) {
      case pheromon::Type::Chase:
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

#ifndef    PHEROMON_FACTORY_HH
# define   PHEROMON_FACTORY_HH

# include "Pheromon.hh"

namespace new_frontiers {

  class PheromonFactory {
    public:

      static
      PheromonShPtr
      newPheromon(const pheromon::Type& type, float x, float y) noexcept;

    private:

      static
      tiles::Effect
      pheromonToVFX(const pheromon::Type& type) noexcept;
  };

}

# include "PheromonFactory.hxx"

#endif    /* PHEROMON_FACTORY_HH */

#ifndef    COLONY_FACTORY_HH
# define   COLONY_FACTORY_HH

# include "Colony.hh"
# include <core_utils/Uuid.hh>

namespace new_frontiers {

  class ColonyFactory {
    public:

      static
      Colony::Props
      newColonyProps(float x, float y, const utils::Uuid& id) noexcept;

    private:

      static const float sk_budget;
      static const float sk_actionCost;
      static const float sk_refill;

      static const colony::Priority sk_focus;

      static const float sk_radius;
      static const int sk_maxSize;
  };

}

# include "ColonyFactory.hxx"

#endif    /* COLONY_FACTORY_HH */

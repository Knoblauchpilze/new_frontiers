#ifndef    COLONY_FACTORY_HXX
# define   COLONY_FACTORY_HXX

# include "ColonyFactory.hxx"
# include "Point.hh"

namespace new_frontiers {

  inline
  Colony::Props
  ColonyFactory::newColonyProps(float x, float y, const utils::Uuid& id) noexcept {
    Colony::Props pp;

    pp.id = id;

    pp.home = newPoint(x, y);

    pp.budget = sk_budget;
    pp.actionCost = sk_actionCost;
    pp.refill = sk_refill;

    pp.focus = sk_focus;

    pp.radius = sk_radius;
    pp.maxSize = sk_maxSize;

    pp.warThreshold = 1u;

    return pp;
  }

}

#endif    /* COLONY_FACTORY_HXX */

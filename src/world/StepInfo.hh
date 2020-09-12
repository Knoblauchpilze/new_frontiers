#ifndef    STEP_INFO_HH
# define   STEP_INFO_HH

# include <vector>
# include <memory>
# include "RNG.hh"
# include "TimeUtils.hh"
# include "Controls.hh"

namespace new_frontiers {

  // Forward declaration to be able to declare variables for
  // the step info class.
  class Entity;
  class VFX;

  using EntityShPtr = std::shared_ptr<Entity>;
  using VFXShPtr = std::shared_ptr<VFX>;

  class Locator;

  using LocatorShPtr = std::shared_ptr<Locator>;

  /**
   * @enum  - Convenience structure regrouping all variables
   *          needed to perform the advancement of one step
   *          of a world object. It includes a RNG, info on
   *          the dimensions of the world, etc.
   */
  struct StepInfo {
    float xMin, xMax;
    float yMin, yMax;

    RNG& rng;

    std::vector<EntityShPtr> eSpawned;
    std::vector<VFXShPtr> vSpawned;

    TimeStamp moment;
    float elapsed;

    LocatorShPtr frustum;

    Controls controls;

    void
    clampCoord(float& x, float& y) const noexcept;
  };

}

# include "StepInfo.hxx"

#endif    /* STEP_INFO_HH */

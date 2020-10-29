#ifndef    STEP_INFO_HH
# define   STEP_INFO_HH

# include <vector>
# include <memory>
# include "RNG.hh"
# include "TimeUtils.hh"
# include "Controls.hh"
# include "Influence.hh"

namespace new_frontiers {

  // Forward declaration to be able to declare variables for
  // the step info class.
  class Block;
  class Entity;
  class VFX;

  using BlockShPtr = std::shared_ptr<Block>;
  using EntityShPtr = std::shared_ptr<Entity>;
  using VFXShPtr = std::shared_ptr<VFX>;

  class Influence;

  using InfluenceShPtr = std::shared_ptr<Influence>;

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

    std::vector<InfluenceShPtr>& influences;

    TimeStamp moment;
    float elapsed;

    LocatorShPtr frustum;

    controls::State controls;

    void
    clampCoord(float& x, float& y) const noexcept;

    /**
     * @brief - Clamp the direction indicated by the input
     *          values so that they describe a valid path
     *          as expected by the world.
     *          Note that we assume that the intial pos of
     *          the path is valid.
     * @param xS - the starting abscissa of the path.
     * @param yS - the starting ordinate of the path.
     * @param xD - the direction of the path in abscissa.
     *             Will be updated.
     * @param yD - the direction of the path in ordinate.
     *             Will be updated.
     * @param d - the length of the path.
     */
    void
    clampPath(float xS, float yS, float& xD, float& yD, float& d) const noexcept;

    void
    spawnBlock(BlockShPtr e);

    void
    removeBlock(Block* e);

    void
    spawnEntity(EntityShPtr e);

    void
    removeEntity(Entity* e);

    void
    spawnVFX(VFXShPtr e);

    void
    removeVFX(VFX* e);
  };

}

# include "StepInfo.hxx"

#endif    /* STEP_INFO_HH */


# include "Entity.hh"
# include "Locator.hh"
# include "LocationUtils.hh"

namespace new_frontiers {

  Entity::Entity(const EntityTile& desc,
                 float radius,
                 float perception,
                 float health):
    Element(desc, radius, health, "entity"),

    m_speed(-1.0f),
    m_perceptionRadius(perception < 0.0f ? 1.0f : perception),
    m_rArrival(0.05f),
    m_pathLength(3.0f),

    m_path(),

    m_state{
      false, // Glowing.
      false  // Exhausted.
    },

    m_passed(),

    m_cPoints()
  {
    // Assign the path with initial position
    // of the entity.
    m_path.xO = m_tile.x;
    m_path.yO = m_tile.y;

    m_path.xT = m_tile.x;
    m_path.yT = m_tile.y;

    m_path.xD = 0.0f;
    m_path.yD = 0.0f;
  }

  bool
  Entity::step(StepInfo& info) {
    // Prepare the entity for this step.
    prepareForStep(info);

# ifdef DEBUG
    if (!isEnRoute()) {
      log(
        "Reached o(" + std::to_string(m_tile.x) + "x" + std::to_string(m_tile.y) +
        "), " + std::to_string(dToT) + " from t(" +
        std::to_string(m_path.xT) + "x" + std::to_string(m_path.yT) + ") (r: " +
        std::to_string(m_rArrival) + ")",
        utils::Level::Debug
      );
    }
# endif
    // Take an action using the behavior
    // function: this is called no matter
    // whether we already have a target.
    // Inheriting classes should take it
    // into account when designing the
    // behaviors.
    choosePath(info);

    // Move along the path.
    bool moved = false;
    if (isEnRoute()) {
      // We know the elapsed time since the
      // last frame, we know the speed of
      // the entity, we can determine the
      // new position.
      m_tile.x += info.elapsed * m_speed * m_path.xD;
      m_tile.y += info.elapsed * m_speed * m_path.yD;

      moved = true;
    }

    // Perform post step operations.
    postStep(info);

    return moved;
  }

  bool
  Entity::isEnRoute() const noexcept {
    float dToT = distance::d(m_path.xT, m_path.yT, m_tile.x, m_tile.y);
    return dToT > m_rArrival;
  }

  void
  Entity::choosePath(StepInfo& info) {
    // Make the entity take an action: this will
    // provide a coordinate to go to.
    float x = 0.0f, y = 0.0f;
    bool anyIdea = takeAction(info, x, y);

    // In case no action was chosen, nothing will
    // happen.
    if (!anyIdea) {
      return;
    }

    // Build the path information from the location
    // that was picked: basically this means clamp
    // any invalid information.
    float xDir = x - m_tile.x;
    float yDir = y - m_tile.y;

    float d = 1.0f;
    normalizePath(info, xDir, yDir, d);

    m_path.xO = m_tile.x;
    m_path.yO = m_tile.y;
    m_path.xT = m_path.xO + d * xDir;
    m_path.yT = m_path.yO + d * yDir;

    m_path.xD = xDir;
    m_path.yD = yDir;
  }

}

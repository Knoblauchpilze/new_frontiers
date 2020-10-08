
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

    m_path.start = now();
    m_path.end = now();
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
      m_path.animate(info.moment, m_tile.x, m_tile.y);
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

    // We start right now: the end time depends on the
    // speed of the entity and the length of the path.
    // Note: we might run into some issues in case the
    // start and end duration are the same: typically
    // the entity will 'jump' from the starting point
    // to the end point. It should not be an issue as
    // the endpoints will be close anyways but still.
    // Remember that.
    m_path.start = info.moment;
    m_path.end = m_path.start + toMilliseconds(static_cast<int>(1000.0f * m_path.length() / m_speed));
  }

}

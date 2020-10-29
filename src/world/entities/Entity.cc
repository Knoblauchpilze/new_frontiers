
# include "Entity.hh"
# include "Locator.hh"
# include "LocationUtils.hh"

namespace new_frontiers {

  Entity::Entity(const Props& props):
    Element(props.tile, props.radius, props.health, "entity", props.owner),

    m_speed(-1.0f),
    m_perceptionRadius(props.perception < 0.0f ? 1.0f : props.perception),
    m_rArrival(props.arrival),
    m_pathLength(props.pathLength),

    m_path(path::newPath(m_tile.x, m_tile.y)),

    m_state{
      false, // Glowing.
      false  // Exhausted.
    },

    m_passed()
  {
    // Nothing to do.
  }

  void
  Entity::step(StepInfo& info) {
    // Prepare the entity for this step.
    prepareForStep(info);

    // Take an action using the behavior
    // function: this is called no matter
    // whether we already have a target.
    // Inheriting classes should take it
    // into account when designing the
    // behaviors.
    choosePath(info);

    // Move along the path.
    if (isEnRoute()) {
      // We know the elapsed time since the
      // last frame, we know the speed of
      // the entity, we can determine the
      // new position.
      m_tile.x += info.elapsed * m_speed * m_path.segments.xD;
      m_tile.y += info.elapsed * m_speed * m_path.segments.yD;
    }

    // Perform post step operations.
    postStep(info);
  }

  bool
  Entity::isEnRoute() const noexcept {
    float dToT = distance::d(m_path.segments.xT, m_path.segments.yT, m_tile.x, m_tile.y);
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
    m_path.segments = path::newSegment(m_tile.x, m_tile.y, xDir, yDir, d);
  }

}

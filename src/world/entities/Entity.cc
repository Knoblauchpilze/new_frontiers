
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

    m_path(path::newPath(m_tile.p)),

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
    if (m_path.enRoute(m_rArrival)) {
      // We know the elapsed time since the
      // last frame, we know the speed of
      // the entity, we can determine the
      // new position.
      m_path.advance(m_speed, info.elapsed, m_rArrival);
      m_tile.p = m_path.cur;
    }

    // Perform post step operations.
    postStep(info);
  }

  void
  Entity::choosePath(StepInfo& info) {
    // Make the entity take an action: this will
    // provide a coordinate to go to.
    takeAction(info, m_path);
  }

}

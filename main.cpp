
/**
 * @brief - Preparation program for the olc code jam starting
 *          on 28/08/2020. The goal is to start using the olc
 *          pixel game engine to produce a small game.
 *
 *          The game: in a haunted world, a valorous knight
 *          is meant to bring a piece of machinery to the
 *          royal engineer. In order to accomplish his task
 *          he needs to traverse the kingdom all the way from
 *          the engineer's valley to the king's palace. Some
 *          enemies have already landed on the kingdom and
 *          the travel is everything but safe.
 *          Across the land, portals are dispersed in order
 *          to connect the multiple dimensions the kingdom
 *          is spanning. The kinght shall pass among all of
 *          them. Demons have quickly understood that these
 *          passages are of the highest importance though
 *          and they began guarding them...
 *
 *          Maybe we could have some sort of dungeon keeper
 *          vibes where the player would have a nest and
 *          could have a menu to produce some units in it.
 *          Each unit would then be selectable and can be
 *          dispatched to some places. When it arrives at
 *          the place, it automatically does something. The
 *          pheromone would be deposited along the way.
 *          Or no selection and just wandering around so
 *          as to have an ants-vibe.
 *          Enemies would follow the same principle to get
 *          to the mother nest and attack it.
 */
// TODO: Create a pheromone class: this class will be laid
// out by entities when they move.
// TODO: Create a base class for all entities that is able
// to lay some pheromones.
// TODO: This entity would have a base protected method to
// select a path based on the amount of pheromone of a type
// around the entity and go in this direction.
// TODO: Refine the spawner to have some sort of abstract
// metric to determine when a new entity should be spawned.
// We could move the current spawner to a time based metric
// to sue the previous mechanism.
// TODO: Create warrior and worker entity classes.
// TODO: Provide a way to select portals with the mouse: it
// would make spawn raw icons to select the priority of the
// entities that would be spawned (between warrior and the
// worker).
// TODO: Maybe change the position of entity so that their
// bottom is at the position indicated by their position ?

# include <core_utils/StdLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "NewFrontiersApp.hh"

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger logger;
  utils::LoggerLocator::provide(&logger);

  logger.setLevel(utils::Level::Verbose);

  const std::string service("new_frontiers");
  const std::string module("main");
  const unsigned width = 640u;
  const unsigned height = 480u;

  try {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Notice,
      std::string("Starting application"),
      module,
      service
    );

    // Define the theme of this application.
    new_frontiers::Theme t;

    t.solidTiles.file = "data/img/gehena.png";
    t.solidTiles.layout = olc::vi2d(8, 3);
    t.portals.file = "data/img/portals.png";
    t.portals.layout = olc::vi2d(16, 2);
    t.entities.file = "data/img/entities.png";
    t.entities.layout = olc::vi2d(16, 4);
    t.vfx.file = "data/img/vfx.png";
    t.vfx.layout = olc::vi2d(14, 1);
    t.cursors.file = "data/img/cursors.png";
    t.cursors.layout = olc::vi2d(2, 1);

    t.size = olc::vi2d(64, 64);

    new_frontiers::NewFrontiersApp demo(width, height, t);
    demo.Start();
  }
  catch (const utils::CoreException& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught internal exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (const std::exception& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (...) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Unexpected error while setting up application"),
      module,
      service
    );
  }

  // All is good.
  return EXIT_SUCCESS;
}


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
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "NewFrontiersApp.hh"

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger logger;
  utils::LoggerLocator::provide(&logger);

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

    t.solidTiles.file = "data/img/crypt.png";
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

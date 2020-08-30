
/**
 * @brief - Preparation program for the olc code jam starting
 *          on 28/08/2020. The goal is to start using the olc
 *          pixel game engine to produce a small game.
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

    new_frontiers::Theme t;
    t.file = "data/img/gehena.png";
    t.layout = olc::vi2d(8, 3);
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

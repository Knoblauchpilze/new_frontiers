
/**
 * @brief - Preparation program for the olc code jam starting
 *          on 28/08/2020. The goal is to start using the olc
 *          pixel game engine to produce a small game.
 *
 *          Textures can be found here:
 *          https://game-icons.net/1x1/delapouite/devil-mask.html
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
 *
 *          Put on hold on December 13th 2020.
 */
// TODO: Quadtree system to represent the elements of the world (in Locator class).

# include <core_utils/log/StdLogger.hh>
# include <core_utils/log/Locator.hh>
# include <core_utils/log/PrefixedLogger.hh>
# include <core_utils/CoreException.hh>
# include "app/IsometricApp.hh"
# include "app/TopViewApp.hh"

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::log::StdLogger raw;
  raw.setLevel(utils::log::Severity::DEBUG);
  utils::log::PrefixedLogger logger("pge", "main");
  utils::log::Locator::provide(&raw);

  try {
    logger.notice("Starting application");

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

# define ISOMETRIC
# ifdef ISOMETRIC
    new_frontiers::IsometricApp demo(olc::vi2d(640, 480), t);
# else
    new_frontiers::TopViewApp demo(olc::vi2d(640, 480));
# endif
    demo.Start();
  }
  catch (const utils::CoreException& e) {
    logger.error("Caught internal exception while setting up application",
                e.what());
  }
  catch (const std::exception& e) {
    logger.error("Caught internal exception while setting up application",
                 e.what());
  }
  catch (...) {
    logger.error("Unexpected error while setting up application");
  }

  // All is good.
  return EXIT_SUCCESS;
}

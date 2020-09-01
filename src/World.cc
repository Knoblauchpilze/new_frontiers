
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <fstream>
# include "Spawner.hh"
# include "Teleporter.hh"
# include "Player.hh"
# include "SolidWall.hh"

namespace new_frontiers {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_tiles(),
    m_entities(),
    m_vfx(),

    m_it(nullptr),
    m_loc(nullptr)
  {
    setService("world");

    // Check dimensions.
    if (m_w <= 3 || m_h <= 3) {
      error(
        std::string("Could not create world"),
        std::string("Invalid dimensions ") + std::to_string(m_w) + "x" + std::to_string(m_h)
      );
    }

    generate();
  }

  World::World(int seed,
               const std::string& file):
    utils::CoreObject("world"),

    m_w(0),
    m_h(0),

    m_rng(seed),

    m_tiles(),
    m_entities(),
    m_vfx(),

    m_it(nullptr),
    m_loc(nullptr)
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
  }


  World::~World() {}

  void
  World::step(float tDelta,
              const std::vector<bool>& controls)
  {
    StepInfo si{
      0.0f,
      1.0f * m_w - 1.0f,
      0.0f,
          1.0f * m_h - 1.0f,

      m_rng,

      std::vector<EntityShPtr>(),
      std::vector<VFXShPtr>(),

      now(),
      tDelta,

      m_loc,

      controls
    };

    // Spawn entities from spawner that can do so.
    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      SolidElementShPtr se = m_tiles[id];

      se->step(si);
    }

    // Register entities that have been spawned. We
    // need to make sure that they're spawned within
    // the world's boundaries.
    m_entities.insert(m_entities.end(), si.eSpawned.cbegin(), si.eSpawned.cend());

    // Make entities evolve.
    bool eMoved = false;
    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      EntityShPtr ep = m_entities[id];

      if (ep->step(si)) {
        // TODO: This might be a real issue in case
        // there are a lot of entities because we're
        // effectively asking a full re-sort on each
        // frame.
        // Maybe we could separate re-sort at the
        // very least.
        eMoved = true;
      }
    }

    // Register vfx that have been created.
    m_vfx.insert(m_vfx.end(), si.vSpawned.cbegin(), si.vSpawned.cend());

    // Make vfx evolve.
    std::vector<int> toRm;

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      VFXShPtr v = m_vfx[id];

      if (v->step(si)) {
        toRm.push_back(id);
      }
    }

    // Remove dead effects. We use reverse iterators as
    // we want to traverse the vector from end to front
    // (so as to keep indices valid while removing) and
    // using a classic for loop would lead to issue if
    // we use an `unsigned` value (we could static_cast
    // but it seems less pretty).
    for (std::vector<int>::reverse_iterator it = toRm.rbegin() ;
         it != toRm.rend() ;
         ++it)
    {
      m_vfx.erase(m_vfx.begin() + toRm[*it]);
    }

    // In case something has been modified, refresh
    // the iterator on this world.
    if (!si.eSpawned.empty() || eMoved || !si.vSpawned.empty() || !toRm.empty()) {
      m_it->refresh();
    }
  }

  void
  World::generateElements() {
    // Generate mob portal.
    SolidTile st = newTile(Portal, 3);
    st.x = 3; st.y = 4;
    m_tiles.push_back(std::make_shared<Spawner>(st, DarkAnubis));

    // Generate entry portal.
    st = newTile(Portal, 6);
    st.x = 1; st.y = 1;
    m_tiles.push_back(std::make_shared<Teleporter>(st));

    // Generate exit portal.
    st = newTile(Portal, 1);
    st.x = 6; st.y = 3;
    m_tiles.push_back(std::make_shared<Teleporter>(st));

    // Generate the player at the same location
    // as the entry portal.
    m_entities.push_back(std::make_shared<Player>(1.0f, 1.0f));
  }

  void
  World::loadFromFile(const std::string& file) {
    // Open the file.
    std::ifstream in(file.c_str());

    if (!in.good()) {
      error(
        std::string("Unable to load level from file \"") + file + "\"",
        std::string("No such file")
      );
    }

    // Read the dimensions of the world.
    loadDimensions(in);

    // Load world's data.
    bool success = true;
    std::string section;

    while (!in.eof() && success) {
      in >> section;

      log("Reading section \"" + section + "\" from \"" + file + "\"");
      if (in.eof()) {
        break;
      }
      else if (section == "entrance") {
        // Starting point of the level.
        loadEntrances(in);
      }
      else if (section == "exit") {
        // End portal of the level.
        loadExits(in);
      }
      else if (section == "portals") {
        // Mob portals for this level.
        loadPortals(in);
      }
      else if (section == "walls") {
        // Walls registered in the level.
        loadWalls(in);
      }
      else if (section == "entities") {
        // Entities already spawned in the level.
        loadEntities(in);
      }
      else {
        // Unknown section.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Failed to decode unknown section \"") + section + "\"",
          utils::Level::Error
        );

        // This is a failure of the parsing.
        success = false;
      }
    }

    if (!success) {
      error(
        std::string("Unable to load level from file \"") + file + "\"",
        std::string("Unexpected section in file")
      );
    }

    // Create an iterator and a locator service from this.
    m_it = std::make_shared<WorldIterator>(m_w, m_h, m_tiles, m_entities, m_vfx);
    m_loc = std::make_shared<WorldElementLocator>(m_w, m_h, m_tiles, m_entities, m_vfx);
  }

  void
  World::loadEntrances(std::ifstream& in) {
    // The entrance section (just like any other section)
    // is ended by the `end` directive.
    std::string section;

    int type;
    float x, y;

    while (!in.eof() && section != "end") {
      in >> section;

      if (in.eof()) {
        break;
      }
      if (section == "end") {
        break;
      }
      if (section == "#") {
        // Comment, continue.
        std::string line;
        std::getline(in, line);

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering entrance ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      SolidTile st = newTile(Portal, type);
      st.x = x; st.y = y;
      m_tiles.push_back(std::make_shared<Teleporter>(st));
    }
  }

  void
  World::loadExits(std::ifstream& in) {
    // The exits section (just like any other section)
    // is ended by the `end` directive.
    std::string section;

    int type;
    float x, y;

    while (!in.eof() && section != "end") {
      in >> section;

      if (in.eof()) {
        break;
      }
      if (section == "end") {
        break;
      }
      if (section == "#") {
        // Comment, continue.
        std::string line;
        std::getline(in, line);

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering exit ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      SolidTile st = newTile(Portal, type);
      st.x = x; st.y = y;
      m_tiles.push_back(std::make_shared<Teleporter>(st));
    }
  }

  void
  World::loadPortals(std::ifstream& in) {
    // The entrance section (just like any other section)
    // is ended by the `end` directive.
    std::string section;

    int type;
    float x, y;
    std::string mobStr;

    while (!in.eof() && section != "end") {
      in >> section;

      if (in.eof()) {
        break;
      }
      if (section == "end") {
        break;
      }
      if (section == "#") {
        // Comment, continue.
        std::string line;
        std::getline(in, line);

        continue;
      }

      in >> type >> x >> y >> mobStr;

      Mob m = strToMob(mobStr);
      if (m == MobsCount) {
        log(
          std::string("Could not decode portal spawning unknown mob \"") + mobStr +
          "\" at " + std::to_string(x) + "x" + std::to_string(y),
          utils::Level::Warning
        );

        continue;
      }

      log(
        std::string("Registering portal ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      SolidTile st = newTile(Portal, type);
      st.x = x; st.y = y;
      m_tiles.push_back(std::make_shared<Spawner>(st, m));
    }
  }

  void
  World::loadWalls(std::ifstream& in) {
    // The exits section (just like any other section)
    // is ended by the `end` directive.
    std::string section;

    int type;
    float x, y;

    while (!in.eof() && section != "end") {
      in >> section;

      if (in.eof()) {
        break;
      }
      if (section == "end") {
        break;
      }
      if (section == "#") {
        // Comment, continue.
        std::string line;
        std::getline(in, line);

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering wall ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      m_tiles.push_back(std::make_shared<SolidWall>(x, y, type));
    }
  }

  void
  World::loadEntities(std::ifstream& /*in*/) {
    /** TODO: Handle this. **/
  }


}


# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <fstream>
# include "blocks/Spawner.hh"
# include "blocks/BlockFactory.hh"
# include "entities/Player.hh"
# include "entities/HostileMob.hh"

namespace {

  /**
   * @brief - Create a valid entity from the input desc.
   *          In case the `kind` or the `type` cannot be
   *          interpreted a null pointer is returned.
   * @param kind - the type of 'brain' to associate to
   *               this entity.
   * @param type - the type of visual display for it.
   * @param id - the variant of the main display.
   * @param x - the abscissa of the entity in cells.
   * @param y - the ordinate of the entity in cells.
   * @return - the pointer on the created entity or a
   *           null pointer if one of the parameter is
   *           not recognized.
   */
  inline
  new_frontiers::EntityShPtr
  createEntity(const std::string& kind,
               const std::string& type,
               int id,
               float x,
               float y)
  {
    // Retrieve the visual display type for the entity.
    new_frontiers::tiles::Entity e = new_frontiers::strToEntity(type);
    if (e == new_frontiers::tiles::EntitiesCount) {
      // We could not interpret the visual display type
      // for this entity.
      return nullptr;
    }

    new_frontiers::EntityTile et;
    et.type = e;
    et.id = id;

    et.x = x;
    et.y = y;

    // Interpret the brain.
    if (kind == "hostile") {
      return std::make_shared<new_frontiers::HostileMob>(et);
    }
    if (kind == "player") {
      return std::make_shared<new_frontiers::Player>(et);
    }

    // Could not interpret the brain.
    return nullptr;
  }

}

namespace new_frontiers {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_blocks(),
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

    m_blocks(),
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
    // Create the step information structure.
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
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      BlockShPtr b = m_blocks[id];

      b->step(si);
    }

    // Register entities that have been spawned.
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
  World::pause(float /*tDelta*/,
               const std::vector<bool>& /*controls*/)
  {
    // Call the pause method on each world element. The
    // pause method is invoked at this moment in time.
    TimeStamp moment = now();

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->pause(moment);
    }

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      m_entities[id]->pause(moment);
    }

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      m_vfx[id]->pause(moment);
    }
  }

  void
  World::resume(float /*tDelta*/,
                const std::vector<bool>& /*controls*/)
  {
    // Call the resume method on each world element. The
    // pause method is invoked at this moment in time.
    TimeStamp moment = now();

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->resume(moment);
    }

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      m_entities[id]->resume(moment);
    }

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      m_vfx[id]->resume(moment);
    }
  }

  void
  World::generateElements() {
    // Generate mob portal.
    m_blocks.push_back(BlockFactory::newSpawner(3, 3.0f, 4.0f, tiles::DarkAnubis));

    // Generate entry portal.
    m_blocks.push_back(BlockFactory::newEntrance(6, 1.0f, 1.0f));

    // Generate exit portal.
    m_blocks.push_back(BlockFactory::newExit(1, 6.0f, 3.0f));

    // Generate the player at the same location
    // as the entry portal.
    EntityTile et = newTile(tiles::Knight, 0);
    et.x = 1.0f; et.y = 1.0f;
    m_entities.push_back(std::make_shared<Player>(et));
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

      // Terminate reading if needed.
      if (in.eof()) {
        break;
      }

      log("Reading section \"" + section + "\" from \"" + file + "\"");

      if (section == "entrances") {
        // Starting point of the level.
        loadEntrances(in);
      }
      else if (section == "exits") {
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
    m_it = std::make_shared<Iterator>(m_w, m_h, m_blocks, m_entities, m_vfx);
    m_loc = std::make_shared<Locator>(m_w, m_h, m_blocks, m_entities, m_vfx);
  }

  void
  World::loadEntrances(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for an entrance is a section called
    // `entrance`.
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
      if (section != "entrance") {
        // Not handled section, continue.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Skipping section \"") + section + "\" (not an entrance)",
          utils::Level::Warning
        );

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering entrance ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      m_blocks.push_back(BlockFactory::newEntrance(type, x, y));
    }
  }

  void
  World::loadExits(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for an exit is a section called `exit`.
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
      if (section != "exit") {
        // Not handled section, continue.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Skipping section \"") + section + "\" (not an exit)",
          utils::Level::Warning
        );

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering exit ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      m_blocks.push_back(BlockFactory::newExit(type, x, y));
    }
  }

  void
  World::loadPortals(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for a portal is a section called `portal`.
    std::string section;

    int type;
    float x, y;
    std::string entStr;

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
      if (section != "portal") {
        // Not handled section, continue.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Skipping section \"") + section + "\" (not a portal)",
          utils::Level::Warning
        );

        continue;
      }

      in >> type >> x >> y >> entStr;

      tiles::Entity e = strToEntity(entStr);
      if (e == tiles::EntitiesCount) {
        log(
          std::string("Could not decode portal spawning unknown entity \"") + entStr +
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

      m_blocks.push_back(BlockFactory::newSpawner(type, x, y, e));
    }
  }

  void
  World::loadWalls(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for a wall is a section called `wall`.
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
      if (section != "wall") {
        // Not handled section, continue.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Skipping section \"") + section + "\" (not a wall)",
          utils::Level::Warning
        );

        continue;
      }

      in >> type >> x >> y;

      log(
        std::string("Registering wall ") +
        std::to_string(type) + " at " +
        std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      m_blocks.push_back(BlockFactory::newWall(type, x, y));
    }
  }

  void
  World::loadEntities(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for an entity is reached upon finding a
    // section called `entity`.
    std::string section;

    // The `type` and `id` caracterize the visual
    // display for the entity: it represents how
    // it will look on screen.
    // The `x` and `y` represent the position of
    // the entity.
    // The `kind` represent whether this entity
    // is a hostile mob, a player, or any other
    // kind of entity. It is more linked to the
    // type of 'brain' that is responsible for
    // its behavior.
    std::string kind;
    std::string type;
    int id;
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
      if (section != "entity") {
        // Not handled section, continue.
        std::string line;
        std::getline(in, line);

        log(
          std::string("Skipping section \"") + section + "\" (not an entity)",
          utils::Level::Warning
        );

        continue;
      }

      in >> kind >> type >> id >> x >> y;

      EntityShPtr e = createEntity(kind, type, id, x, y);
      if (e == nullptr) {
        log(
          std::string("Could not decode entity with unknown kind \"") + kind +
          "\" and type \"" + type + "\"",
          utils::Level::Warning
        );

        continue;
      }

      m_entities.push_back(e);
    }
  }


}
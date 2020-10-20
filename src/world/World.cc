
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <fstream>
# include "blocks/Spawner.hh"
# include "blocks/BlockFactory.hh"
# include "entities/EntityFactory.hh"
# include "entities/Player.hh"
# include "entities/Mob.hh"
# include "colonies/ColonyFactory.hh"
# include "TimeUtils.hh"

# include <iostream>

namespace {

  inline
  new_frontiers::EntityShPtr
  createEntity(const std::string& kind,
               const std::string& mob,
               float x,
               float y,
               float health,
               float homeX,
               float homeY,
               float carrying,
               float cargo,
               float attack = 0.0f)
  {
    // Retrieve the visual display type for the entity.
    // In case none is found, return an empty pointer.
    new_frontiers::tiles::Entity e = new_frontiers::strToEntity(mob);
    if (e == new_frontiers::tiles::EntitiesCount) {
      std::cout << "[HAHA] " << mob << std::endl;
      return nullptr;
    }

    // Interpret the brain, assign custom properties
    // and return the created object.
    if (kind == "warrior") {
      new_frontiers::Warrior::WProps pp = new_frontiers::EntityFactory::newWarriorProps(x, y, e);
      pp.health = health;

      pp.homeX = homeX + 0.5f;
      pp.homeY = homeY + 0.5f;
      pp.carrying = carrying;
      pp.cargo = cargo;

      pp.attack = attack;

      return std::make_shared<new_frontiers::Warrior>(pp);
    }
    if (kind == "worker") {
      new_frontiers::Mob::MProps pp = new_frontiers::EntityFactory::newWorkerProps(x, y, e);
      pp.health = health;

      pp.homeX = homeX + 0.5f;
      pp.homeY = homeY + 0.5f;
      pp.carrying = carrying;
      pp.cargo = cargo;

      return std::make_shared<new_frontiers::Worker>(pp);
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

    m_colonies(),
    m_blocks(),
    m_entities(),
    m_vfx(),

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

    m_colonies(),
    m_blocks(),
    m_entities(),
    m_vfx(),

    m_loc(nullptr)
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
  }

  void
  World::step(float tDelta,
              const Controls& controls)
  {
    // Create the step information structure.
    StepInfo si{
      0.0f,
      1.0f * m_w - 1.0f,
      0.0f,
      1.0f * m_h - 1.0f,

      m_rng,

      std::vector<InfluenceShPtr>(),

      now(),
      tDelta,

      m_loc,

      controls
    };

    // Make elements evolve.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->step(si);
    }

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      m_entities[id]->step(si);
    }

    for (unsigned id = 0u ; id < m_vfx.size() ; ++id) {
      m_vfx[id]->step(si);
    }

    // Finally make colonies evolve.
    for (unsigned id = 0u ; id < m_colonies.size() ; ++id) {
      m_colonies[id]->step(si);
    }

    // Process influences.
    processInfluences(si.influences);
  }

  void
  World::pause(float /*tDelta*/,
               const Controls& /*controls*/)
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
                const Controls& /*controls*/)
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
    // Generate the colonies.
    Colony::Props c = ColonyFactory::newColonyProps(1.0f, 4.0f, utils::Uuid::create());
    c.focus = colony::Priority::Expansion;
    c.refill = 0.2f;
    m_colonies.push_back(std::make_shared<Colony>(c));

    c = ColonyFactory::newColonyProps(2.0f, 2.0f, utils::Uuid::create());
    c.focus = colony::Priority::Consolidation;
    c.refill = 0.4f;
    m_colonies.push_back(std::make_shared<Colony>(c));

    c = ColonyFactory::newColonyProps(2.0f, 1.0f, utils::Uuid::create());
    c.focus = colony::Priority::War;
    c.refill = 0.05f;
    m_colonies.push_back(std::make_shared<Colony>(c));

    // Generate resource deposit.
    Deposit::DProps dp = BlockFactory::newDepositProps(3.0f, 4.0f);
    dp.stock = 20.0f;
    m_blocks.push_back(BlockFactory::newDeposit(dp));

    // Generate the player at the same location
    // as the entry portal.
    Player::PProps plp = EntityFactory::newPlayerProps(1.0f, 1.0f, tiles::Knight);
    m_entities.push_back(std::make_shared<Player>(plp));
  }

  void
  World::processInfluences(const std::vector<InfluenceShPtr>& influences) {
    // Process each influence.
    std::size_t bCount = m_blocks.size();

    for (unsigned id = 0; id < influences.size() ; ++id) {
      InfluenceShPtr i = influences[id];

      switch (i->getType()) {
        case influence::Type::BlockSpawn:
          m_blocks.push_back(i->getShPBlock());
          break;
        case influence::Type::BlockRemoval: {
          auto toRm = std::find_if(
            m_blocks.cbegin(),
            m_blocks.cend(),
            [&i](const BlockShPtr& blo) {
              return blo != nullptr && blo.get() == i->getBlock();
            }
          );
          if (toRm != m_blocks.end()) {
            m_blocks.erase(toRm);
          }
          } break;
        case influence::Type::EntitySpawn:
          m_entities.push_back(i->getShPEntity());
          break;
        case influence::Type::EntityRemoval: {
          auto toRm = std::find_if(
            m_entities.cbegin(),
            m_entities.cend(),
            [&i](const EntityShPtr& ent) {
              return ent != nullptr && ent.get() == i->getEntity();
            }
          );
          if (toRm != m_entities.end()) {
            m_entities.erase(toRm);
          }
          } break;
        case influence::Type::VFXSpawn:
          m_vfx.push_back(i->getShPVFX());
          break;
        case influence::Type::VFXRemoval: {
          auto toRm = std::find_if(
            m_vfx.cbegin(),
            m_vfx.cend(),
            [&i](const VFXShPtr& vfx) {
              return vfx != nullptr && vfx.get() == i->getVFX();
            }
          );
          if (toRm != m_vfx.end()) {
            m_vfx.erase(toRm);
          }
          } break;
        default:
          log("Unhandled influence with type " + std::to_string(static_cast<int>(i->getType())), utils::Level::Warning);
          break;
      }
    }

    // In case the number of blocks has been changed
    // we need to update the locator.
    if (m_blocks.size() != bCount) {
      m_loc->refresh();
    }
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

      if (section == "portals") {
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

    m_loc = std::make_shared<Locator>(m_w, m_h, m_blocks, m_entities, m_vfx, m_colonies);
  }

  void
  World::loadPortals(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for a portal is a section called `portal`.
    std::string section;

    std::string mobStr, mobTypeStr;
    float x, y, cost, stock, refill;

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

        log("Skipping section \"" + section + "\" (not a portal)", utils::Level::Warning);
        continue;
      }

      // TODO: We should handle the owner's string.
      in >> x >> y >> mobStr >> mobTypeStr >> cost >> stock >> refill;

      // Convert mob type from string to enumeration.
      tiles::Entity e = strToEntity(mobStr);
      if (e == tiles::EntitiesCount) {
        log(
          "Could not decode portal spawning unknown entity \"" + mobStr +
          "\" at " + std::to_string(x) + "x" + std::to_string(y),
          utils::Level::Warning
        );

        continue;
      }

      // Convert agent kind from string to enumeration.
      bool err = false;
      mob::Type mt = mob::strToType(mobTypeStr, err);
      if (err) {
        log(
          "Could not decode portal spawning unknown entity type \"" + mobTypeStr +
          "\" at " + std::to_string(x) + "x" + std::to_string(y),
          utils::Level::Warning
        );

        continue;
      }

      log(
        "Registering portal spawning " + mobStr + " at " + std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      // Note that we only allow creation of portals
      // with a spawner-o-meter type for now.
      SpawnerOMeter::SOMProps pp = BlockFactory::newSpawnerOMeterProps(x, y, e);
      pp.agent = mt;

      pp.threshold = cost;
      pp.reserve = stock;
      pp.refill = refill;

      m_blocks.push_back(BlockFactory::newSpawnerOMeter(pp));
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

        log("Skipping section \"" + section + "\" (not a wall)", utils::Level::Warning);
        continue;
      }

      in >> type >> x >> y;

      log(
        "Registering wall " + std::to_string(type) + " at " + std::to_string(x) + "x" + std::to_string(y),
        utils::Level::Verbose
      );

      m_blocks.push_back(BlockFactory::newWall(x, y, type));
    }
  }

  void
  World::loadEntities(std::ifstream& in) {
    // Parse until we reach a `end` directive. The
    // cue for an entity is reached upon finding a
    // section called `entity`.
    std::string section;

    std::string kind, mob;
    float x, y, health, homeX, homeY, carrying, cargo, attack;

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

        log("Skipping section \"" + section + "\" (not an entity)", utils::Level::Warning);
        continue;
      }

      // TODO: We should handle the owner's string.
      in >> kind >> mob >> x >> y >> health >> homeX >> homeY >> carrying >> cargo >> attack;

      EntityShPtr e = createEntity(kind, mob, x, y, health, homeX, homeY, carrying, cargo, attack);
      if (e == nullptr) {
        log("Could not decode entity with unknown kind \"" + kind + "\"", utils::Level::Warning);
        continue;
      }

      m_entities.push_back(e);
    }
  }

}

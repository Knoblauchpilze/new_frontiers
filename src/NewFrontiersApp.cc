
# define OLC_PGE_APPLICATION

# include "NewFrontiersApp.hh"
# include "utils.hh"

namespace new_frontiers {

  NewFrontiersApp::NewFrontiersApp(unsigned width,
                                   unsigned height,
                                   const std::string& appName):
    utils::CoreObject(appName),
    olc::PixelGameEngine(),

    m_sprite(nullptr),
    m_aliases(),

    m_portals(),
    m_enemies(),

    m_random(),

    m_first(true)
  {
    sAppName = appName;
    setService("app");

    initialize(width, height);

    generatePortals();
    for (unsigned id = 0u ; id < ENEMIES_COUNT ; ++id) {
      spawnMonster();
    }
  }

  NewFrontiersApp::~NewFrontiersApp() {
    delete m_sprite;
  }

  bool
  NewFrontiersApp::OnUserCreate() {

    return true;
  }

  bool
  NewFrontiersApp::OnUserUpdate(float /*fElapsedTime*/) {
    // Clear rendering target.
    Clear(olc::VERY_DARK_GREEN);
    SetPixelMode(olc::Pixel::ALPHA);

    // Draw ground.
    bool full = m_aliases[Sprite::Ground].full;
    for (unsigned y = 0u ; y < WORLD_SIZE_H ; ++y) {
      for (unsigned x = 0u ; x < WORLD_SIZE_W ; ++x) {
        if (m_first && x == 3u && y == 3u) {
          log("Painting ground at " + toString(olc::vi2d(x, y)) + " at " + toString(tileCoordsToPixels(olc::vi2d(x, y), full)));
        }
        DrawPartialSprite(tileCoordsToPixels(olc::vi2d(x, y), full), m_sprite, m_aliases[Sprite::Ground].coords, spriteSize());
      }
    }

    // Draw portals.
    full = m_aliases[Sprite::Portal].full;
    for (unsigned id = 0u ; id < m_portals.size() ; ++id) {
      olc::vi2d coord = m_portals[id];
      DrawPartialSprite(tileCoordsToPixels(coord, full), m_sprite, m_aliases[Sprite::Portal].coords, spriteSize());
    }

    // Draw door.
    DrawPartialSprite(tileCoordsToPixels(olc::vi2d(1, 1), m_aliases[Sprite::Door].full), m_sprite, m_aliases[Sprite::Door].coords, spriteSize());

    // Draw enemies.
    full = m_aliases[Sprite::Monster].full;
    for (unsigned id = 0u ; id < m_enemies.size() ; ++id) {
      olc::vi2d coord = m_enemies[id];
      DrawPartialSprite(tileCoordsToPixels(coord, full), m_sprite, m_aliases[Sprite::Monster].coords, spriteSize());
    }

    // Draw cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = pixelCoordsToTiles(mp);
    olc::vi2d ctp = tileCoordsToPixels(mtp, m_aliases[Sprite::Cursor].full);
    DrawPartialSprite(ctp, m_sprite, m_aliases[Sprite::Cursor].coords, spriteSize());

    SetPixelMode(olc::Pixel::NORMAL);

    DrawString(olc::vi2d(0, 450), "Mouse coords        : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, 465), "World cell coords   : " + toString(mtp), olc::CYAN);

    m_first = false;

    return true;
  }

  void
  NewFrontiersApp::initialize(unsigned width,
                              unsigned height)
  {
    // Construct the window. Note that we use a pixel size
    // to screen size ratio of `1` (meaning that each pixel
    // of the viewport will be represented by a pixel on
    // the screen).
    olc::rcode c = Construct(width, height, 1, 1);

    if (c != olc::OK) {
      throw utils::CoreException(
        std::string("Could not build new frontiers application"),
        std::string("Initialization failed")
      );
    }

    createTileAliases();

    // Initialize random generator.
    m_random.rng.seed(m_random.device());
    m_random.reset(WORLD_SIZE_W - 2, WORLD_SIZE_H - 2);
  }

  void
  NewFrontiersApp::createTileAliases() {
    m_sprite = new olc::Sprite("data/img/64x64.png");

    m_aliases.resize(TileCount);

    m_aliases[Sprite::Portal] = Tile{spriteCoordsToPixels(12, 5), false};
    m_aliases[Sprite::Monster] = Tile{spriteCoordsToPixels(6, 7), true};
    m_aliases[Sprite::Ground] = Tile{spriteCoordsToPixels(10, 4), false};
    m_aliases[Sprite::Door] = Tile{spriteCoordsToPixels(1, 2), false};
    m_aliases[Sprite::Cursor] = Tile{spriteCoordsToPixels(5, 10), false};
  }

  void
  NewFrontiersApp::generatePortals() {
    for (unsigned id = 0u ; id < PORTALS_COUNT ; ++id) {
      olc::vi2d coord = m_random.coords();

      log("Creating portal at " + toString(coord), utils::Level::Info);
      m_portals.push_back(coord);
    }
  }

  void
  NewFrontiersApp::spawnMonster() {
    olc::vi2d coord = m_random.coords();

    log("Spawning enemy at " + toString(coord), utils::Level::Info);
    m_enemies.push_back(coord);
  }

}

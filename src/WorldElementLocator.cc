
# include "WorldElementLocator.hxx"

namespace {

  template <typename T>
  inline
  void
  swap(T& t1, T& t2) {
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
  }

}

namespace new_frontiers {

  WorldElementLocator::WorldElementLocator(int width,
                                           int height,
                                           const std::vector<SolidElementShPtr>& tiles,
                                           const std::vector<EntityShPtr>& entities,
                                           const std::vector<VFXShPtr>& vfx):
    utils::CoreObject("locator"),

    m_w(width),
    m_h(height),

    m_tiles(tiles),
    m_entities(entities),
    m_vfx(vfx),

    m_solidIDs()
  {
    setService("world");

    initialize();
  }

  int
  WorldElementLocator::countEntities(const Mob& mob, float x, float y, float radius) const {
    int count = 0;

    // Traverse entities and search for `mob`s.
    float r2 = radius * radius;

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      const EntityTile& et = m_entities[id]->getTile();

      if (et.type == mob && d2(x, y, et.x, et.y) <= r2) {
        ++count;
      }
    }

    return count;
  }

  bool
  WorldElementLocator::obstructed(float x, float y, float xDir, float yDir, float d) const noexcept {
    // We basically need to find which cells are
    // 'under' the line when it spans its path
    // so as to determine whether there is some
    // solid tile along the way.
    // Finding the closest pixel is not an easy
    // task in and on itself and we will use a
    // Bresenham as defined here:
    // https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham.
    // We handle only one special case: we do
    // not consider that the first cell can be
    // obstructed (that is the `(x, y)` cell).

    // First, we need to compute the endpoints
    // of the line segment to trace.
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);

    // TODO: Handle cases where the first cell is obstructed.

    int x1 = static_cast<int>(x + d * xDir);
    int y1 = static_cast<int>(y + d * yDir);

    log(
      "Requested s(" + std::to_string(x0) + "x" + std::to_string(y0) +
      ") to e(" + std::to_string(x1) + "x" + std::to_string(y1) + ")",
      utils::Level::Verbose
    );

    bool obstruction = false;

    // First, handle cases where the line is
    // perfectly horizontal or vertical.
    if (x0 == x1 && y0 == y1) {
      // Line in a single cell: not possible
      // to be obstructed ever.
      log("Line is a single point at " + std::to_string(x0) + "x" + std::to_string(y0) + ", obstruction never reported", utils::Level::Verbose);
      return false;
    }

    if (x0 == x1) {
      // Vertical line.
      int sY = std::min(y0, y1);
      int maxY = std::max(y0, y1);

      while (!obstruction && sY <= maxY) {
        obstruction = (sY != y0) && (m_solidIDs.count(sY * m_w + x0) > 0);

        log(
          "Check v at " + std::to_string(x0) + "x" + std::to_string(sY) +
          ": " + std::to_string(m_solidIDs.count(sY * m_w + x0)) +
          " o: " + std::to_string(obstruction),
          utils::Level::Verbose
        );

        ++sY;
      }

      return obstruction;
    }

    if (y0 == y1) {
      // Horizontal line.
      int sX = std::min(x0, x1);
      int maxX = std::max(x0, x1);

      while (!obstruction && sX <= maxX) {
        obstruction = (sX != x0) && (m_solidIDs.count(y0 * m_w + sX) > 0);

        log(
          "Check h at " + std::to_string(sX) + "x" + std::to_string(y0) +
          ": " + std::to_string(m_solidIDs.count(y0 * m_w + sX)) +
          " o: " + std::to_string(obstruction),
          utils::Level::Verbose
        );

        ++sX;
      }

      return obstruction;
    }

    // We know now that the line is neither
    // horizontal nor vertical: we can with
    // no loss of generality swap around `x`
    // and `y` variables in order to always
    // have a line going in the positive
    // direction in both `x` and `y` axes.
    // This allows to have a single general
    // case for the algorithm.
    int oX, oY, sX, sY, eX, eY, dx, dy, err;

    enum Op {
      LT,
      LTE,
      GT,
      GTE
    };

    Op o = LT;

    dx = x1 - x0;
    dy = y1 - y0;

    oX = x0;
    oY = y0;

    int oct = 0;

    if (dx > 0) {
      if (dy > 0) {
        if (dx >= dy) {
          // Vector close to the horizontal (1st octant).
          // Canonical case, nothing to change from the
          // default settings.
          oct = 1;
        }
        else {
          // Vector close to the vertical (2nd octant).
          swap(x0, y0);
          swap(x1, y1);

          swap(oX, oY);

          swap(dx, dy);
          oct = 2;
        }
      }
      else {
        if (dx >= -dy) {
          // Vector close to the horizontal (8th octant).
          // TODO: This would seem to indicate that when
          // we're doing `dx/y *= -1` it is not needed to
          // to `swap(x/y, x/y1)` on top of it ?
          // But maybe it makes sense to change it all to
          // have some sort of traversal of the path with
          // a delta and cast at regular interval to see
          // how it behaves.
          // Would probably give better results anyway.
          // swap(y0, y1);
          dy *= -1;
          oct = 8;
        }
        else {
          // Vector close to the vertical (7th octant).
          swap(x0, y0);
          swap(x1, y1);

          swap(oX, oY);

          swap(dx, dy);

          swap(x0, x1);
          dx *= -1;
          o = GT;
          oct = 7;
        }
      }
    }
    else {
      if (dy > 0) {
        if (-dx >= dy) {
          // Vector close to the horizontal (4th octant).
          swap(x0, x1);

          dy *= -1;
          o = GTE;
          oct = 4;
        }
        else {
          // Vector close to the vertical (3rd octant).
          swap(x0, x1);

          swap(dx, dy);
          dx *= -1;
          o = LTE;
          oct = 3;
        }
      }
      else {
        if (dx <= dy) {
          // Vector close to the horizontal (5th octant).
          swap(x0, x1);

          swap(y0, y1);
          o = GTE;
          oct = 5;
        }
        else {
          // Vector close to the vertical (6th octant).
          swap(x0, x1);

          swap(y0, y1);
          o = GTE;

          swap(dx, dy);
          oct = 6;
        }
      }
    }

    // TODO: This fails miserably.
    // Attempt o(0.551082x2.771852) to t(1.677884x1.108787)
    // Requested s(0x2) to e(1x1)
    // Analysis oct8 s(0x1) to e(1x2)
    // Check at 0x1: 0 o: 0
    // Check e at 1x2: 0 o: 0

    sX = x0; sY = y0;
    eX = x1; eY = y1;

    err = dx;

    dx *= 2;
    dy *= 2;

    log(
      "Analysis oct" + std::to_string(oct) +
      " s(" + std::to_string(x0) + "x" + std::to_string(y0) +
      ") to e(" + std::to_string(x1) + "x" + std::to_string(y1) + ")",
      utils::Level::Verbose
    );

    while (!obstruction && sX < eX) {
      err -= dy;

      log(
        "Check at " + std::to_string(sX) + "x" + std::to_string(sY) +
        ": " + std::to_string(m_solidIDs.count(sY * m_w + sX)) +
        " o: " + std::to_string(m_solidIDs.count(sY * m_w + sX) > 0),
        utils::Level::Verbose
      );

      obstruction = (sX != oX || sY != oY) && (m_solidIDs.count(sY * m_w + sX) > 0);

      // Check for vertical displacement.
      if ((o == LT && err < 0) ||
          (o == LTE && err <= 0) ||
          (o == GT && err > 0) ||
          (o == GTE && err >= 0))
      {
        ++sY;
        err += dx;
      }


      ++sX;
    }

    if (obstruction) {
      return true;
    }

    log(
      "Check e at " + std::to_string(eX) + "x" + std::to_string(eY) +
      ": " + std::to_string(m_solidIDs.count(sY * m_w + sX)) +
      " o: " + std::to_string(m_solidIDs.count(eY * m_w + eX) > 0),
      utils::Level::Verbose
    );

    // The algorithm does not check the last
    // pixel so we finally return the result
    // of the test.
    return (m_solidIDs.count(eY * m_w + eX) > 0);
  }

  void
  WorldElementLocator::initialize() {
    // Register each solid tile in the map.
    for (unsigned id = 0u ; id < m_tiles.size() ; ++id) {
      const SolidTile& st = m_tiles[id]->getTile();
      
      m_solidIDs.insert(static_cast<int>(st.y) * m_w + static_cast<int>(st.x));
    }
  }

}

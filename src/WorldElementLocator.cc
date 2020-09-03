
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
  WorldElementLocator::obstructed(float x, float y, float xDir, float yDir, float d, std::vector<float>& cPoints) const noexcept {
    // We basically need to find which cells are
    // 'under' the line when it spans its path
    // so as to determine whether there is some
    // solid tile along the way.
    // At first, using a Bresenham algorithm to
    // compute the cells that lie under a path
    // seemed like a good idea. But in the end
    // it appear that we are usually computing
    // small path where we basically just trace
    // two pixels and the performance of the
    // collision detection was not sufficient
    // to rule out some of them.
    // So instead we figured we would sample
    // the path and take regular probing at
    // the underlying grid. As pathes are on
    // average small, we won't do a lot of
    // comparison and it is reasonable to do
    // so. It also provide more control on
    // precisely where we sample the path.
    // We chose to use half a tile dims as
    // a sample path in order to be precise
    // enough.
    float xT = x + d * xDir;
    float yT = y + d * yDir;

    cPoints.clear();

    int xo = static_cast<int>(x);
    int yo = static_cast<int>(y);

    // Compute the step to add on the path
    // for each sampling: it is extracted
    // from the initial normalized that we
    // normalize.
    // We handle the trivial case where the
    // direction does not have a valid length
    // in which case we return `false` (as in
    // no obstructed) as the initial cell is
    // never considered obstructed.
    float l = std::sqrt(xDir * xDir + yDir * yDir);

    if (l < 0.0001f) {

      log(
        "Checking dir(" + std::to_string(xDir) + "x" + std::to_string(yDir) + ")" +
        " not enough to change tile, not obstructed",
        utils::Level::Verbose
      );

      return false;
    }

    xDir /= l;
    yDir /= l;

    // Sample the path in steps of at most a
    // length of half the cell.
    xDir /= 2.0f;
    yDir /= 2.0f;

    bool obstruction = false;
    float t = 0.0f;

    int xi, yi;

    while (!obstruction && t < d) {
      // Prevent the initial cell to be considered
      // as obstructed: this allow objects that get
      // stuck to be able to move out.
      xi = static_cast<int>(x);
      yi = static_cast<int>(y);

      cPoints.push_back(x);
      cPoints.push_back(y);

      obstruction = (xi != xo || yi != yo) && (m_solidIDs.count(yi * m_w + xi) > 0);

      log(
        "Checking " + std::to_string(x) + "x" + std::to_string(y) +
        " i(" + std::to_string(xi) + "x" + std::to_string(yi) + ")" +
        ": " + std::to_string(obstruction) +
        " t: " + std::to_string(t) + " perc: " + std::to_string(t / d),
        utils::Level::Verbose
      );

      x += xDir;
      y += yDir;

      t += 0.5f;
    }

    // In case an obstruction was detected we
    // don't need to check for the last cell.
    if (obstruction) {
      return true;
    }

    // Check obstruction for the final cell.
    xi = static_cast<int>(xT);
    yi = static_cast<int>(yT);

    cPoints.push_back(xT);
    cPoints.push_back(yT);

    log(
      "Checking " + std::to_string(xT) + "x" + std::to_string(yT) +
      " i(" + std::to_string(xi) + "x" + std::to_string(yi) + ") (end)" +
      ": " + std::to_string(m_solidIDs.count(yi * m_w + xi) > 0),
      utils::Level::Verbose
    );

    return (m_solidIDs.count(yi * m_w + xi) > 0);
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


# include "Locator.hxx"
# include "LocationUtils.hh"

namespace new_frontiers {

  Locator::Locator(int width,
                   int height,
                   const std::vector<BlockShPtr>& blocks,
                   const std::vector<EntityShPtr>& entities,
                   const std::vector<VFXShPtr>& vfxs):
    utils::CoreObject("locator"),

    m_w(width),
    m_h(height),

    m_blocks(blocks),
    m_entities(entities),
    m_vfxs(vfxs),

    m_blocksIDs()
  {
    setService("world");

    initialize();
  }

  int
  Locator::countEntities(const tiles::Entity& ent, float x, float y, float radius) const {
    int count = 0;

    // Traverse entities and search for `mob`s.
    float r2 = radius * radius;

    for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
      const EntityTile& et = m_entities[id]->getTile();

      if (et.type == ent && distance::d2(x, y, et.x, et.y) <= r2) {
        ++count;
      }
    }

    return count;
  }

  bool
  Locator::obstructed(float x, float y, float xDir, float yDir, float d, std::vector<float>& cPoints) const noexcept {
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
# ifdef DEBUG
      log(
        "Checking dir(" + std::to_string(xDir) + "x" + std::to_string(yDir) + ")" +
        " not enough to change tile, not obstructed",
        utils::Level::Verbose
      );
# endif

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

      obstruction = (xi != xo || yi != yo) && (m_blocksIDs.count(yi * m_w + xi) > 0);

# ifdef DEBUG
      log(
        "Checking " + std::to_string(x) + "x" + std::to_string(y) +
        " i(" + std::to_string(xi) + "x" + std::to_string(yi) + ")" +
        ": " + std::to_string(obstruction) +
        " t: " + std::to_string(t) + " perc: " + std::to_string(t / d),
        utils::Level::Verbose
      );
# endif

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

# ifdef DEBUG
    log(
      "Checking " + std::to_string(xT) + "x" + std::to_string(yT) +
      " i(" + std::to_string(xi) + "x" + std::to_string(yi) + ") (end)" +
      ": " + std::to_string(m_blocksIDs.count(yi * m_w + xi) > 0),
      utils::Level::Verbose
    );
# endif

    return (m_blocksIDs.count(yi * m_w + xi) > 0);
  }

  std::vector<world::ItemEntry>
  Locator::getVisible(float xMin,
                      float yMin,
                      float xMax,
                      float yMax,
                      world::ItemType* type,
                      bool sort) const noexcept
  {
    std::vector<world::ItemEntry> out;
    std::vector<SortEntry> entries;

    world::ItemEntry ie;

    // Traverse first the blocks if needed.
    if (type == nullptr || *type == world::ItemType::Block) {
      ie.type = world::ItemType::Block;

      for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
        const BlockTile& t = m_blocks[id]->getTile();

        if (t.x < xMin || t.x > xMax || t.y < yMin || t.y > yMax) {
          continue;
        }

        ie.index = id;
        entries.push_back(SortEntry{t.x, t.y, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Then entities.
    if (type == nullptr || *type == world::ItemType::Entity) {
      ie.type = world::ItemType::Entity;

      for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
        const EntityTile& t = m_entities[id]->getTile();

        if (t.x < xMin || t.x > xMax || t.y < yMin || t.y > yMax) {
          continue;
        }

        ie.index = id;
        entries.push_back(SortEntry{t.x, t.y, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // And finally vfxs.
    if (type == nullptr || *type == world::ItemType::VFX) {
      ie.type = world::ItemType::VFX;

      for (unsigned id = 0u ; id < m_vfxs.size() ; ++id) {
        const VFXTile& t = m_vfxs[id]->getTile();

        if (t.x < xMin || t.x > xMax || t.y < yMin || t.y > yMax) {
          continue;
        }

        ie.index = id;
        entries.push_back(SortEntry{t.x, t.y, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Check whether we need to sort the output
    // vector.
    if (sort) {
      // Sort the entries by ascending `z` order.
      auto cmp = [](const SortEntry& lhs, const SortEntry& rhs) {
        return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
      };

      std::sort(entries.begin(), entries.end(), cmp);

      // Reorder the output vector based on the
      // result of the sort.
      std::vector<world::ItemEntry> sorted;
      sorted.swap(out);

      for (unsigned id = 0u ; id < entries.size() ; ++id) {
        out.push_back(sorted[entries[id].id]);
      }
    }

    return out;
  }

  std::vector<BlockShPtr>
  Locator::getBlocks(float x,
                     float y,
                     float r,
                     const tiles::Block& block,
                     int id,
                     bool sort) const noexcept
  {
    std::vector<BlockShPtr> out;

    float r2 = r * r;

    // Search for the block in the internal list.
    for (unsigned i = 0u ; i < m_blocks.size() ; ++i) {
      const BlockTile& b = m_blocks[i]->getTile();

      if (b.type != block || (id >= 0 && b.id != id) || m_blocks[i]->isDead()) {
        continue;
      }

      // Consider the center of the block for the
      // distance to the input `x` and `y` coords.
      if (r < 0.0f || distance::d2(b.x + 0.5f, b.y + 0.5f, x, y) < r2) {
        out.push_back(m_blocks[i]);
      }
    }

    // Sort if needed.
    if (sort) {
      std::sort(
        out.begin(),
        out.end(),
        [&x, &y](const BlockShPtr& lhs, const BlockShPtr& rhs) {
          float d1 = distance::d2(
            lhs->getTile().x + 0.5f,
            lhs->getTile().y + 0.5f,
            x,
            y
          );

          float d2 = distance::d2(
            rhs->getTile().x + 0.5f,
            rhs->getTile().y + 0.5f,
            x,
            y
          );

          return d1 < d2;
        }
      );
    }

    return out;
  }

  std::vector<EntityShPtr>
  Locator::getEntities(float x,
                       float y,
                       float r,
                       const tiles::Entity& ent,
                       int id,
                       bool sort) const noexcept
  {
    std::vector<EntityShPtr> out;

    float r2 = r * r;

    // Search for the block in the internal list.
    for (unsigned i = 0u ; i < m_entities.size() ; ++i) {
      const EntityTile& e = m_entities[i]->getTile();

      if (e.type != ent || (id >= 0 && e.id != id) || m_entities[i]->isDead()) {
        continue;
      }

      float dx = e.x - x;
      float dy = e.y - y;

      if (r < 0.0f || dx * dx + dy * dy < r2) {
        out.push_back(m_entities[i]);
      }
    }

    // Sort if needed.
    if (sort) {
      std::sort(
        out.begin(),
        out.end(),
        [&x, &y](const EntityShPtr& lhs, const EntityShPtr& rhs) {
          float dx1 = lhs->getTile().x - x;
          float dy1 = lhs->getTile().y - y;

          float dx2 = rhs->getTile().x - x;
          float dy2 = rhs->getTile().y - y;

          return (dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2);
        }
      );
    }

    return out;
  }

  void
  Locator::initialize() {
    // Register each solid tile in the map.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      const BlockTile& bt = m_blocks[id]->getTile();

      m_blocksIDs.insert(static_cast<int>(bt.y) * m_w + static_cast<int>(bt.x));
    }
  }

}

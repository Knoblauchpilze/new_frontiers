
# include "Locator.hxx"
# include "LocationUtils.hh"

namespace new_frontiers {

  Locator::Locator(int width,
                   int height,
                   const std::vector<BlockShPtr>& blocks,
                   const std::vector<EntityShPtr>& entities,
                   const std::vector<VFXShPtr>& vfxs,
                   const std::vector<ColonyShPtr>& colonies):
    utils::CoreObject("locator"),

    m_w(width),
    m_h(height),

    m_blocks(blocks),
    m_entities(entities),
    m_vfxs(vfxs),
    m_colonies(colonies),

    m_blocksIDs()
  {
    setService("world");

    initialize();
  }

  bool
  Locator::obstructed(float x,
                      float y,
                      float xDir,
                      float yDir,
                      float d,
                      std::vector<float>& cPoints,
                      float* xObs,
                      float* yObs) const noexcept
  {
    // We basically need to find which cells are 'under' the
    // line when it spans its path so as to determine whether
    // there is some solid tile along the way.
    // At first, using a Bresenham algorithm to compute the
    // cells that lie under a path seemed like a good idea.
    // But in the end it appears that we are usually computing
    // small path where we basically just trace two pixels and
    // the performance of the collision detection was not
    // sufficient to rule out some of them.
    // So instead we figured we would sample the path and take
    // regular probing at the underlying grid. As pathes are on
    // average small, we won't do a lot of comparison and it is
    // reasonable to do so. It also provide more control on
    // precisely where we sample the path.
    // We chose to use half a tile dims as a sample path in
    // order to be precise enough.
    float xT = x + d * xDir;
    float yT = y + d * yDir;

    int xo = static_cast<int>(x);
    int yo = static_cast<int>(y);

    // Compute the step to add on the path for each sampling:
    // it is extracted from the initial normalized that we
    // normalize.
    // We handle the trivial case where the direction does not
    // have a valid length in which case we return `false` (as
    // in no obstructed) as the initial cell is never considered
    // obstructed.
    float l = std::sqrt(xDir * xDir + yDir * yDir);
    if (l < 0.0001f) {
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

// # define DEBUG
# ifdef DEBUG
      std::cout << "[LOC] Considering " << x << "x" << y
                << " which " << (obstruction ? "is" : "is not")
                << " obstructed (" << t << ", " << (t / d) << "%)"
                << std::endl;
# endif

      if (!obstruction) {
        x += xDir;
        y += yDir;

        t += 0.5f;
      }
    }

    // In case an obstruction was detected we
    // don't need to check for the last cell.
    if (obstruction) {
      if (xObs != nullptr) {
        *xObs = xi;
      }
      if (yObs != nullptr) {
        *yObs = yi;
      }

      return true;
    }

    // Check obstruction for the final cell.
    xi = static_cast<int>(xT);
    yi = static_cast<int>(yT);

    cPoints.push_back(xT);
    cPoints.push_back(yT);

    obstruction = (m_blocksIDs.count(yi * m_w + xi) > 0);
    if (obstruction) {
      if (xObs != nullptr) {
        *xObs = xi;
      }
      if (yObs != nullptr) {
        *yObs = yi;
      }
    }

    return obstruction;
  }

  std::vector<world::ItemEntry>
  Locator::getVisible(float xMin,
                      float yMin,
                      float xMax,
                      float yMax,
                      const world::ItemType* type,
                      const world::Filter* filter,
                      world::Sort sort) const noexcept
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

        // Check the owner: if the filtering is defined we
        // we want to reject the item if:
        //  - the filter says to include the specified id
        //    and the item's one is different.
        //  - the filter says to exclude the specified id
        //    and the item's one is identical.
        const utils::Uuid& uuid = m_blocks[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
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

        // See above for details.
        const utils::Uuid& uuid = m_entities[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
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

        // See above for details.
        const utils::Uuid& uuid = m_vfxs[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
          continue;
        }

        ie.index = id;
        entries.push_back(SortEntry{t.x, t.y, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Check whether we need to sort the output
    // vector.
    if (sort != world::Sort::None) {
      // Sort the entries by ascending `z` order.
      auto cmp = [](const SortEntry& lhs, const SortEntry& rhs) {
        // Note that we will actually always sort by
        // `z` order: indeed we don't have any ref
        // point to sort by distance so it would be
        // pointless anyway.
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

  std::vector<world::ItemEntry>
  Locator::getVisible(float x,
                      float y,
                      float r,
                      const world::ItemType* type,
                      const world::Filter* filter,
                      world::Sort sort) const noexcept
  {
    std::vector<world::ItemEntry> out;
    std::vector<SortEntry> entries;

    world::ItemEntry ie;
    float r2 = r * r;

    // Traverse first the blocks if needed.
    if (type == nullptr || *type == world::ItemType::Block) {
      ie.type = world::ItemType::Block;

      for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
        const BlockTile& t = m_blocks[id]->getTile();

        if (r > 0.0f && distance::d2(t.x + 0.5f, t.y + 0.5f, x, y) > r2) {
          continue;
        }

        // Check the owner: if the filtering is defined we
        // we want to reject the item if:
        //  - the filter says to include the specified id
        //    and the item's one is different.
        //  - the filter says to exclude the specified id
        //    and the item's one is identical.
        const utils::Uuid& uuid = m_blocks[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
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

        if (r > 0.0f && distance::d2(t.x, t.y, x, y) > r2) {
          continue;
        }

        // See above for details.
        const utils::Uuid& uuid = m_entities[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
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

        if (r > 0.0f && distance::d2(t.x, t.y, x, y) > r2) {
          continue;
        }

        // See above for details.
        const utils::Uuid& uuid = m_vfxs[id]->getOwner();
        if (filter != nullptr &&
            (
              (filter->include && uuid != filter->id) ||
              (!filter->include && uuid == filter->id)
            )
           )
        {
          continue;
        }

        ie.index = id;
        entries.push_back(SortEntry{t.x, t.y, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Check whether we need to sort the output
    // vector.
    if (sort != world::Sort::None) {
      // Sort the entries by ascending `z` order.
      auto cmp = [&sort, &x, &y](const SortEntry& lhs, const SortEntry& rhs) {
        switch (sort) {
          case world::Sort::Distance:
            return distance::d(x, y, lhs.x, lhs.y) < distance::d(x, y, rhs.x, rhs.y);
          case world::Sort::ZOrder:
            // Use `z` order as default sorting alg
            // in case the input is unknown.
          default:
            return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
        }
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

  void
  Locator::initialize() {
    // Register each solid tile in the map.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      const BlockTile& bt = m_blocks[id]->getTile();

      m_blocksIDs.insert(static_cast<int>(bt.y) * m_w + static_cast<int>(bt.x));
    }
  }

}

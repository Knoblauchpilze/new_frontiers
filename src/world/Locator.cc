
# include "Locator.hxx"
# include <maths_utils/LocationUtils.hh>

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
  Locator::obstructed(utils::Point2f p,
                      float xDir,
                      float yDir,
                      float d,
                      std::vector<utils::Point2f>& cPoints,
                      utils::Point2f* obs,
                      float sample,
                      bool allowLog) const noexcept
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
    utils::Point2f end;
    end.x() = p.x() + d * xDir;
    end.y() = p.y() + d * yDir;

    int xo = static_cast<int>(p.x());
    int yo = static_cast<int>(p.y());

    // Handle the trivial case where the direction does not
    // have a valid length in which case we return `false`
    // (as in no obstructed) as the initial cell is never
    // considered obstructed.
    if (std::sqrt(xDir * xDir + yDir * yDir) < 0.0001f) {
      return false;
    }

    bool obstruction = false;
    float t = 0.0f;

    int xi, yi;

    if (allowLog) {
      verbose(
        "Start: " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
        ", end: " + std::to_string(end.x()) + "x" + std::to_string(end.y()) +
        ", l: " + std::to_string(d) +
        ", dir: " + std::to_string(xDir) + "x" + std::to_string(yDir) +
        ", (norm: " + std::to_string(std::sqrt(xDir * xDir + yDir * yDir)) + ")"
      );
    }

    xDir *= d;
    yDir *= d;

    while (!obstruction && t < 1.0f) {
      // Prevent the initial cell to be considered
      // as obstructed: this allow objects that get
      // stuck to be able to move out.
      xi = static_cast<int>(p.x());
      yi = static_cast<int>(p.y());

      cPoints.push_back(p);

      obstruction = (xi != xo || yi != yo) && (m_blocksIDs.count(yi * m_w + xi) > 0);

      if (allowLog) {
        verbose(
          "Considering " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
          " which " + (obstruction ? "is" : "is not") +
          " obstructed (" + std::to_string(t) + ", " + std::to_string(100.0f * t) +
          "%, d: " + std::to_string(d) + ")"
        );
      }

      if (!obstruction) {
        p.x() += sample * xDir;
        p.y() += sample * yDir;

        t += sample;
      }
    }

    // In case an obstruction was detected we
    // don't need to check for the last cell.
    if (obstruction) {
      if (obs != nullptr) {
        *obs = p;
      }

      if (allowLog) {
        verbose(
          "Found obstruction at " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
          " (" + std::to_string(t) + ", " + std::to_string(100.0f * t) +
          "%, d: " + std::to_string(d) + ")"
        );
      }

      return true;
    }

    // Check obstruction for the final cell.
    xi = static_cast<int>(end.x());
    yi = static_cast<int>(end.y());

    cPoints.push_back(end);

    obstruction = (m_blocksIDs.count(yi * m_w + xi) > 0);
    if (obstruction) {
      if (obs != nullptr) {
        *obs = end;
      }
    }

    if (allowLog) {
      verbose(
        std::string("") + (obstruction ? "Found" : "Didn't find") +
        " obstruction 2 at " +
        std::to_string(end.x()) + "x" + std::to_string(end.y()) +
        " (" + std::to_string(t) + ", " + std::to_string(100.0f * t) +
        "%, d: " + std::to_string(d) + ")"
      );
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

        if (t.p.x() < xMin || t.p.x() > xMax || t.p.y() < yMin || t.p.y() > yMax) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Then entities.
    if (type == nullptr || *type == world::ItemType::Entity) {
      ie.type = world::ItemType::Entity;

      for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
        const EntityTile& t = m_entities[id]->getTile();

        if (t.p.x() < xMin || t.p.x() > xMax || t.p.y() < yMin || t.p.y() > yMax) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // And finally vfxs.
    if (type == nullptr || *type == world::ItemType::VFX) {
      ie.type = world::ItemType::VFX;

      for (unsigned id = 0u ; id < m_vfxs.size() ; ++id) {
        const VFXTile& t = m_vfxs[id]->getTile();

        if (t.p.x() < xMin || t.p.x() > xMax || t.p.y() < yMin || t.p.y() > yMax) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
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
        return lhs.p.x() < rhs.p.x() || (lhs.p.x() == rhs.p.x() && lhs.p.y() < rhs.p.y());
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
  Locator::getVisible(const utils::Point2f& p,
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

        if (r > 0.0f && utils::d2(t.p.x() + 0.5f, t.p.y() + 0.5f, p.x(), p.y()) > r2) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Then entities.
    if (type == nullptr || *type == world::ItemType::Entity) {
      ie.type = world::ItemType::Entity;

      for (unsigned id = 0u ; id < m_entities.size() ; ++id) {
        const EntityTile& t = m_entities[id]->getTile();

        if (r > 0.0f && utils::d2(t.p.x(), t.p.y(), p.x(), p.y()) > r2) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // And finally vfxs.
    if (type == nullptr || *type == world::ItemType::VFX) {
      ie.type = world::ItemType::VFX;

      for (unsigned id = 0u ; id < m_vfxs.size() ; ++id) {
        const VFXTile& t = m_vfxs[id]->getTile();

        if (r > 0.0f && utils::d2(t.p, p) > r2) {
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
        entries.push_back(SortEntry{t.p, static_cast<unsigned>(out.size())});
        out.push_back(ie);
      }
    }

    // Check whether we need to sort the output
    // vector.
    if (sort != world::Sort::None) {
      // Sort the entries by ascending `z` order.
      auto cmp = [&sort, &p](const SortEntry& lhs, const SortEntry& rhs) {
        switch (sort) {
          case world::Sort::Distance:
            return utils::d(p, lhs.p) < utils::d(p, rhs.p);
          case world::Sort::ZOrder:
            // Use `z` order as default sorting alg
            // in case the input is unknown.
          default:
            return lhs.p.x() < rhs.p.x() || (lhs.p.x() == rhs.p.x() && lhs.p.y() < rhs.p.y());
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
      m_blocksIDs.insert(static_cast<int>(bt.p.y()) * m_w + static_cast<int>(bt.p.x()));
    }
  }

}

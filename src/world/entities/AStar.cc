
# include "AStar.hh"
# include <deque>

# include <iostream>

namespace {

  /**
   * @brief - Enumeration describing the position of a node
   *          relatively to its neighbots.
   */
  enum Neighbor {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
    Count
  };

  /**
   * @brief - Convenience structure to define an opened node.
   */
  struct Node {
    new_frontiers::Point p;
    float c;
    float h;

    bool
    contains(const new_frontiers::Point& p) const noexcept;

    std::vector<Node>
    generateNeighbors(const new_frontiers::Point& target) const noexcept;

    int
    hash(int offset) const noexcept;

    static
    new_frontiers::Point
    invertHash(int hash, int offset) noexcept;
  };

  inline
  bool
  Node::contains(const new_frontiers::Point& m) const noexcept {
    int iPX = static_cast<int>(p.x);
    int iPY = static_cast<int>(p.y);

    int iMX = static_cast<int>(m.x);
    int iMY = static_cast<int>(m.y);

    return iPX == iMX && iPY == iMY;
  }

  inline
  std::vector<Node>
  Node::generateNeighbors(const new_frontiers::Point& target) const noexcept {
    std::vector<Node> neighbors(Count);

    new_frontiers::Point np;

    float bx = std::floor(p.x) + 0.5f;
    float by = std::floor(p.y) + 0.5f;

    // Right neighbor.
    np.x = bx + 1.0f; np.y = by;
    neighbors[East] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Top right neighbor.
    np.x = bx + 1.0f; np.y = by + 1.0f;
    neighbors[NorthEast] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Up neighbor.
    np.x = bx; np.y = by + 1.0f;
    neighbors[North] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Top left neighbor.
    np.x = bx - 1.0f; np.y = by + 1.0f;
    neighbors[NorthWest] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Left neighbor.
    np.x = bx - 1.0f; np.y = by;
    neighbors[West] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Bottom left neighbor.
    np.x = bx - 1.0f; np.y = by - 1.0f;
    neighbors[SouthWest] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Down neighbor.
    np.x = bx; np.y = by - 1.0f;
    neighbors[South] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    // Bottom right neighbor.
    np.x = bx + 1.0f; np.y = by - 1.0f;
    neighbors[SouthEast] = Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    };

    return neighbors;
  }

  inline
  int
  Node::hash(int offset) const noexcept {
    return static_cast<int>(std::floor(p.y)) * offset + static_cast<int>(std::floor(p.x));
  }

  inline
  new_frontiers::Point
  Node::invertHash(int hash, int offset) noexcept {
    new_frontiers::Point p;

    p.x = 0.5f + hash % offset;
    p.y = 0.5f + hash / offset;

    return p;
  }

}

namespace new_frontiers {

  AStar::AStar(const Point& s,
               const Point& e,
               LocatorShPtr loc):
    utils::CoreObject("algo"),

    m_start(s),
    m_end(e),

    m_loc(loc)
  {
    setService("astar");
  }

  bool
  AStar::findPath(std::vector<new_frontiers::Point>& path, bool allowLog) const noexcept {
    // The code for this algorithm has been taken from the
    // below link:
    // https://en.wikipedia.org/wiki/A*_search_algorithm
    path.clear();

    // The list of nodes that are currently being explored.
    std::vector<Node> nodes;
    std::deque<int> openNodes;
    Node init{m_start, 0.0f, distance::d(m_start, m_end)};
    nodes.push_back(init);
    openNodes.push_back(0);

    if (allowLog) {
      log(
        "Starting a* at " + std::to_string(m_start.x) + "x" + std::to_string(m_start.y) +
        " to reach " + std::to_string(m_end.x) + "x" + std::to_string(m_end.y)
      );
    }

    using AssociationMap = std::unordered_map<int, int>;

    // The `cameFrom[i]` defines the index of its parent
    // node, i.e. the node we were traversing when this
    // node was encountered.
    AssociationMap cameFrom;
    AssociationMap associations;

    associations[init.hash(m_loc->w())] = init.hash(m_loc->w());

    // Common lambdas to handle sorting and distance
    // computation from a point to another.
    auto cmp = [&nodes](int lhs, int rhs) {
      const Node& nlhs = nodes[lhs];
      const Node& nrhs = nodes[rhs];
      return nlhs.c + nlhs.h < nrhs.c + nrhs.h;
    };

    while (!openNodes.empty()) {
      // Sort the open list to fetch nodes with smallest
      // `c + h` value.
      std::sort(openNodes.begin(), openNodes.end(), cmp);
      Node current = nodes[openNodes.front()];
      openNodes.pop_front();

      if (allowLog) {
        log(
          "Picked node " + std::to_string(current.p.x) + "x" + std::to_string(current.p.y) +
          " with c " + std::to_string(current.c) +
          " h is " + std::to_string(current.h) +
          " (nodes: " + std::to_string(openNodes.size()) + ")"
        );
      }

      // In case we reached the goal, stop there.
      if (current.contains(m_end)) {
        if (allowLog) {
          log(
            "Found path to " + std::to_string(m_end.x) + "x" + std::to_string(m_end.y) +
            " with c " + std::to_string(current.c) + ", h " + std::to_string(current.h)
          );
        }

        bool found = reconstructPath(cameFrom, m_loc->w(), path, allowLog);
        if (found) {
          // Smooth out the sharp turns that might have
          // been produced by the A*.
          smoothPath(path);
        }

        return found;
      }

      // Also, consider the node if it is not obstructed
      // in a less obvious way as below:
      //
      // +----+----+
      // | Ob |  E |
      // |    |    |
      // +----+----+
      // |    | Ob |
      // | S  |    |
      // +----+----+
      //
      // This situation can only happen for the diagonal
      // neighbors (so either SE, SW, NE, NW).
      // We will first determine before processing the
      // neighbors and check the status for each one.
      std::vector<Node> neighbors = current.generateNeighbors(m_end);

      float bx = std::floor(current.p.x) + 0.5f;
      float by = std::floor(current.p.y) + 0.5f;

      bool obsE = m_loc->obstructed(Point{bx + 1.0f, by});
      bool obsN = m_loc->obstructed(Point{bx, by + 1.0f});
      bool obsW = m_loc->obstructed(Point{bx - 1.0f, by});
      bool obsS = m_loc->obstructed(Point{bx, by - 1.0f});

      bool validNE = !obsN || !obsE;
      bool validNW = !obsN || !obsW;
      bool validSW = !obsS || !obsW;
      bool validSE = !obsS || !obsE;

      for (unsigned id = 0u ; id < neighbors.size() ; ++id) {
        Node& neighbor = neighbors[id];

        // Only consider the node if it is not obstructed.
        if (m_loc->obstructed(neighbor.p) && !neighbor.contains(m_end)) {
          continue;
        }

        // Use the pre-computed diagonal neighbors status
        // and prevent the registration of the it if it
        // is not valid in the sense defined in the above
        // section.
        if ((id == NorthEast && !validNE) ||
            (id == SouthEast && !validSE) ||
            (id == SouthWest && !validSW) ||
            (id == NorthWest && !validNW))
        {
          continue;
        }

        AssociationMap::iterator it = associations.find(neighbor.hash(m_loc->w()));

        if (it == associations.end() || neighbor.c < nodes[it->second].c) {
          // This path to neighbor is better than any previous one.
          cameFrom[neighbor.hash(m_loc->w())] = current.hash(m_loc->w());

          if (it != associations.end()) {
            if (allowLog) {
              log(
                "Updating " + std::to_string(neighbor.p.x) + "x" + std::to_string(neighbor.p.y) +
                " from c " + std::to_string(nodes[it->second].c) + ", " + std::to_string(nodes[it->second].h) +
                " (f: " + std::to_string(nodes[it->second].c + nodes[it->second].h) + "," +
                " parent: " + std::to_string(cameFrom[nodes[it->second].hash(m_loc->w())]) + ")" +
                " to c: " + std::to_string(neighbor.c) + " h: " + std::to_string(neighbor.h) +
                " (f: " + std::to_string(neighbor.c + neighbor.h) + "," +
                " parent is " + std::to_string(current.hash(m_loc->w())) + ")"
              );
            }

            nodes[it->second].c = neighbor.c;
          }
          else {
            openNodes.push_back(nodes.size());
            associations[neighbor.hash(m_loc->w())] = nodes.size();
            nodes.push_back(neighbor);
          }
        }
      }
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstructPath(const std::unordered_map<int, int>& parents,
                         int offset,
                         std::vector<Point>& path,
                         bool allowLog) const noexcept
  {
    std::vector<Point> out;

    Node n{m_end, 0.0f, 0.0f};
    int h = n.hash(offset);
    std::unordered_map<int, int>::const_iterator it = parents.find(h);

    while (it != parents.cend()) {
      Point p = Node::invertHash(h, offset);

      if (allowLog) {
        log(
          "Registering point " + std::to_string(p.x) + "x" + std::to_string(p.y) +
          " with hash " + std::to_string(h) +
          ", parent is " + std::to_string(it->second)
        );
      }

      out.push_back(p);
      h = it->second;
      it = parents.find(h);
    }

    // Make sure that we reached the starting point.
    // If this is the case we can copy the path we
    // just built to the output argument.
    n.p = m_start;
    int sh = n.hash(offset);

    if (sh == h) {
      // We need to reverse the path as we've built
      // it from the end.
      for (std::vector<Point>::const_reverse_iterator it = out.crbegin() ;
           it != out.crend() ;
           ++it)
      {
        path.push_back(*it);
      }
    }

    return sh == h;
  }

  void
  AStar::smoothPath(std::vector<Point>& path) const noexcept {
    // The basic idea is taken from this very interesting
    // article found in Gamasutra:
    // https://www.gamasutra.com/view/feature/131505/toward_more_realistic_pathfinding.php?page=2
    // The idea is to remove progressively intermediate
    // positions on the path as long as the next step can
    // be reached with a straight line from the previous
    // point.
    // The starting point of the path is already the first
    // element in the `path` so we can start from there.
    // We will eliminate the trivial case where the path
    // is emtpy right away: this corresponds to all paths
    // that don't have at least 3 elements (and thus where
    // no simplification can occur).
    if (path.size() < 2u) {
      return;
    }

    std::vector<Point> out;
    Point p = m_start;

    unsigned id = 0u;
    std::vector<Point> dummy;

    // Simplify the whole path.
    int count = 0;
    while (id < path.size() - 1u && count < 10) {
      Point c = path[id + 1u];
      Node n{c, 0.0f, 0.0f};

      // Check whether the starting position and the
      // current point can be joined by a straight
      // line without obstructions. Note that we will
      // ignore obstructions in the target.
      if (!m_loc->obstructed(p, c, dummy) || n.contains(m_end)) {
        // The path can be reached in a straight line,
        // we can remove the current point.
# ifdef DEBUG
        log(
          "Simplified point " + std::to_string(path[id].x) + "x" + std::to_string(path[id].y)
        );
# endif
        ++id;
      }
      else {
        // Can't reach the point from the current start.
        // This segment cannot be simplified further.
# ifdef DEBUG
        log(
          "Can't simplify path from " + std::to_string(p.x) + "x" + std::to_string(p.y) +
          " to point " + std::to_string(c.x) + "x" + std::to_string(c.y) +
          " (id: " + std::to_string(id) + ", s: " + std::to_string(path.size()) + ")" +
          " registering " + std::to_string(p.x) + "x" + std::to_string(p.y)
        );
# endif
        out.push_back(p);
        p = path[id];
      }

      ++count;
    }

    // Register the last points.
    out.push_back(p);
    out.push_back(path.back());

    // Swap the simplified path with the input argument.
    path.swap(out);
  }

}

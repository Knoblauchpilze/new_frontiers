
# include "AStar.hh"
# include <deque>
# include <iostream>

namespace {

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
    std::vector<Node> neighbors;

    new_frontiers::Point np;

    float bx = std::floor(p.x) + 0.5f;
    float by = std::floor(p.y) + 0.5f;

    // Right neighbor.
    np.x = bx + 1.0f; np.y = by;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Up neighbor.
    np.x = bx; np.y = by + 1.0f;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Left neighbor.
    np.x = bx - 1.0f; np.y = by;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Down neighbor.
    np.x = bx; np.y = by - 1.0f;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    return neighbors;
  }

  inline
  int
  Node::hash(int offset) const noexcept {
    return static_cast<int>(p.y) * offset + static_cast<int>(p.x);
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
  AStar::findPath(std::vector<new_frontiers::Point>& path) const noexcept {
    // The code for this algorithm has been taken from the
    // below link:
    // https://en.wikipedia.org/wiki/A*_search_algorithm
    path.clear();

    // The list of nodes that are currently being explored.
    std::deque<Node> openNodes;
    Node init{m_start, 0.0f, 0.0f};
    openNodes.push_back(init);

    std::cout << "[a*] Starting a* at " << m_start.x << "x" << m_start.y
              << " to reach " << m_end.x << "x" << m_end.y
              << std::endl;

    // The `cameFrom[i]` defines the index of its parent
    // node, i.e. the node we were traversing when this
    // node was encountered.
    std::unordered_map<int, int> cameFrom;

    using ScoreMap = std::unordered_map<int, float>;

    // The `gScore[i]` is the best score reached so far
    // for any node. The default value being infinity
    // (if it is not registered in the map).
    ScoreMap gScore;
    gScore[init.hash(m_loc->w())] = 0.0f;

    // The `fScore[i]` is our current best guess at how
    // short a path from start to finish can be if it
    // passes through `i`.
    ScoreMap fScore;
    fScore[init.hash(m_loc->w())] = init.h;

    // Common lambdas to handle sorting and distance
    // computation from a point to another.
    auto cmp = [](const Node& lhs, const Node& rhs) {
      return lhs.c + lhs.h < rhs.c + rhs.h;
    };

    auto d = [](const Node& lhs, const Node& rhs) {
      return distance::d(lhs.p, rhs.p);
    };

    int count = 0;
    while (!openNodes.empty() && count < 50) {
      // Sort the open list to fetch nodes with smallest
      // `c + h` value.
      std::sort(openNodes.begin(), openNodes.end(), cmp);
      Node current = openNodes.front();

      // In case we reached the goal, stop there.
      if (current.contains(m_end)) {
        std::cout << "[a*] Found path to " << m_end.x << "x" << m_end.y << std::endl;
        return reconstructPath(cameFrom, m_loc->w(), path);
      }

      openNodes.pop_front();

      std::vector<Node> neighbors = current.generateNeighbors(m_end);

      std::cout << "[a*][pick] Picked node " << current.p.x << "x" << current.p.y
                << " with c " << current.c
                << " h is " << current.h
                << " (nodes: " << openNodes.size() << ")"
                << std::endl;

      for (unsigned id = 0u ; id < neighbors.size() ; ++id) {
        // The `d(current,neighbor)` is the weight of the edge from
        // current to neighbor `gScoreAttempt` is the distance from
        // start to the neighbor through `current`.
        Node& neighbor = neighbors[id];
        float gScoreAttempt = gScore[current.hash(m_loc->w())] + d(current, neighbor);

        ScoreMap::iterator it = gScore.find(neighbor.hash(m_loc->w()));

        if (it == gScore.end() || gScoreAttempt < it->second) {
          // This path to neighbor is better than any previous one.
          cameFrom[neighbor.hash(m_loc->w())] = current.hash(m_loc->w());
          gScore[neighbor.hash(m_loc->w())] = gScoreAttempt;


          std::cout << "[a*] Updating " << neighbor.p.x << "x" << neighbor.p.y
                    << " from c " << neighbor.c << " to " << gScoreAttempt
                    << " and f score to " << (gScore[neighbor.hash(m_loc->w())] + neighbor.h)
                    << " parent is " << current.hash(m_loc->w())
                    << std::endl;

          neighbor.c = gScoreAttempt;
          fScore[neighbor.hash(m_loc->w())] = gScore[neighbor.hash(m_loc->w())] + neighbor.h;

          if (it == gScore.end()) {
            openNodes.push_back(neighbor);
          }
        }
      }

      ++count;
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstructPath(const std::unordered_map<int, int>& parents,
                         int offset,
                         std::vector<Point>& path) const noexcept
  {
    std::vector<Point> out(1u, m_end);

    Node n{m_end, 0.0f, 0.0f};
    int h = n.hash(offset);
    std::unordered_map<int, int>::const_iterator it = parents.find(h);

    while (it != parents.cend()) {
      Point p = Node::invertHash(h, offset);
      std::cout << "[a*] Registering point " << p.x << "x" << p.y
                << " with hash " << h
                << ", parent is " << it->first
                << std::endl;
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

}

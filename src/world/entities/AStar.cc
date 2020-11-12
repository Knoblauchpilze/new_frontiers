
# include "AStar.hh"
# include <deque>
# include <iostream>
# include <unordered_map>

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

    // Right neighbor.
    np.x = p.x + 1.0f; np.y = p.y;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Up neighbor.
    np.x = p.x; np.y = p.y + 1.0f;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Left neighbor.
    np.x = p.x - 1.0f; np.y = p.y;
    neighbors.push_back(Node{
      np,
      c + new_frontiers::distance::d(p, np),
      new_frontiers::distance::d(np, target)
    });

    // Down neighbor.
    np.x = p.x; np.y = p.y - 1.0f;
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
    return static_cast<int>(p.x) * offset + static_cast<int>(p.x);
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

    // The `cameFrom[i]` defines the index of its parent
    // node, i.e. the node we were traversing when this
    // node was encountered.
    std::vector<int> cameFrom;

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
      return lhs.c + lhs.h < rhs.c + lhs.h;
    };

    auto d = [](const Node& lhs, const Node& rhs) {
      return distance::d(lhs.p, rhs.p);
    };

    while (!openNodes.empty()) {
      // Sort the open list to fetch nodes with smallest
      // `c + h` value.
      std::sort(openNodes.begin(), openNodes.end(), cmp);
      Node current = openNodes.front();

      // In case we reached the goal, stop there.
      if (current.contains(m_end)) {
        std::cout << "[ASTAR] Found path to " << m_end.x << "x" << m_end.y << std::endl;
        return reconstructPath(path);
      }

      openNodes.pop_front();

      std::vector<Node> neighbors = current.generateNeighbors(m_end);

      for (unsigned id = 0u ; id < neighbors.size() ; ++id) {
        // The `d(current,neighbor)` is the weight of the edge from
        // current to neighbor `gScoreAttempt` is the distance from
        // start to the neighbor through `current`.
        Node& neighbor = neighbors[id];
        float gScoreAttempt = gScore[current.hash(m_loc->w())] + d(current, neighbor);

        ScoreMap::iterator it = gScore.find(neighbor.hash(m_loc->w()));
        if (it == gScore.end() || gScoreAttempt < it->second) {
          // This path to neighbor is better than any previous one.
          cameFrom[neighbor] = current;
          gScore[neighbor.hash(m_loc->w())] = gScoreAttempt;
          neighbor.c = gScoreAttempt;
          fScore[neighbor.hash(m_loc->w())] = gScore[neighbor.hash(m_loc->w())] + neighbor.h;

          if (it == gScore.end()) {
            openNodes.push_back(neighbor);
          }
        }
      }
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstructPath(std::vector<Point>& /*path*/) const noexcept {
    // TODO: Handle this.
    return false;
  }

}

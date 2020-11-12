#ifndef    ASTAR_HH
# define   ASTAR_HH

# include <core_utils/CoreObject.hh>
# include "Point.hh"
# include "Locator.hh"

namespace new_frontiers {

  class AStar: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new A* capable object allowing to
       *          go from the starting point `s` to the end
       *          point `e` using the world described by the
       *          locator.
       * @param s - the starting position.
       * @param e - the end position.
       * @param loc - the locator describing the world.
       */
      AStar(const Point& s,
            const Point& e,
            LocatorShPtr loc);

      /**
       * @brief - Used to generate the path from the start
       *          position to the end and return the path
       *          that was built in the output argument.
       *          In case no path can be found, the return
       *          value will indicate so.
       * @param path - the path generated to reach the two
       *               end points of this finder.
       * @return - `true` if a path could be find.
       */
      bool
      findPath(std::vector<Point>& path) const noexcept;

    private:

      /**
       * @brief - Used to reconstruct the path stored in
       *          the object assuming that we found a valid
       *          path from the ending point.
       * @param path - output vector which will contain the
       *               reconstructed path.
       * @return - `true` if the path could be reconstructed.
       */
      bool
      reconstructPath(std::vector<Point>& path) const noexcept;

    private:

      /**
       * @brief - The starting point of the algorithm.
       */
      Point m_start;

      /**
       * @brief - The end point of the algorithm.
       */
      Point m_end;

      /**
       * @brief - A locator object allowing to determine
       *          whether a location is obstructed.
       */
      LocatorShPtr m_loc;
  };

}

# include "AStar.hxx"

#endif    /* ASTAR_HH */

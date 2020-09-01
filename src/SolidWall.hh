#ifndef    SOLID_WALL_HH
# define   SOLID_WALL_HH

# include <memory>
# include "SolidElement.hh"

namespace new_frontiers {

  class SolidWall: public SolidElement {
    public:

      /**
       * @brief - Create a new wall from the input coordinates
       *          and variant.
       * @param x - the abscissa of this wall in cells.
       * @param y - the ordinate of this wall in cells.
       * @param id - the index of the variant of this wall.
       */
      SolidWall(float x, float y, int id = 0);
  };

  using SolidWallShPtr = std::shared_ptr<SolidWall>;
}

# include "SolidWall.hxx"

#endif    /* SOLID_WALL_HH */

#ifndef    SOLID_WALL_HXX
# define   SOLID_WALL_HXX

# include "SolidWall.hh"

namespace new_frontiers {

  SolidWall::SolidWall(float x, float y, int id):
    SolidElement(newTile(Wall, id), "wall")
  {
    m_tile.x = x;
    m_tile.y = y;
  }

}

#endif    /* SOLID_WALL_HXX */

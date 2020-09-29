#ifndef    CELL_LOCATION_HH
# define   CELL_LOCATION_HH

namespace new_frontiers {

  /**
   * @brief - Define the type of conversion from cell
   *          to pixels. Allow to determine whether
   *          the output position should correspond
   *          to the center, the top left corner, the
   *          center, etc. of the cell.
   */
  enum class Cell {
    TopLeft,      //< The input position is assumed to refer
                  //< to the top left corner of the tile.
    Center,       //< The input position refers to the center
                  //< of the tile.
    CenterBottom, //< The input position refers to the bottom
                  //< center of the tile.
    CenterLeft    //< The input position refers to the center
                  //< of the left side of the tile.
  };

}

#endif    /* CELL_LOCATION_HH */

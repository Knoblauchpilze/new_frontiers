#ifndef    CELL_LOCATION_HH
# define   CELL_LOCATION_HH

namespace new_frontiers {

  /**
   * @brief - Define the type of conversion from cell
   *          to pixels. Allow to determine whether
   *          the output position should correspond
   *          to the center, the top left corner, the
   *          center, etc. of the cell.
   *          This is usually used as first computing
   *          a main position which references the top
   *          left corner for this cell and performing
   *          a conversion so that the output position
   *          actually corresponds to the expected
   *          location in the cell.
   *          Specifying `TopLeft` usually prevents
   *          any additional transformation to be
   *          performed.
   */
  enum class Cell {
    TopLeft,      //< The input position is assumed to refer
                  //< to the top left corner of the tile.
    CenterBottom, //< The input position refers to the bottom
                  //< center of the tile.
    UpperLeft     //< Corresponds to the left side of the tile
                  //< half a tile above the top left corner.
  };

}

#endif    /* CELL_LOCATION_HH */

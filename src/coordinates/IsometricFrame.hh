#ifndef    ISOMETRIC_FRAME_HH
# define   ISOMETRIC_FRAME_HH

# include "CoordinateFrame.hh"

namespace new_frontiers {

  class IsometricFrame: public CoordinateFrame {
    public:

      /**
       * @brief - Defines a new isometric frame with the specified
       *          viewport and tile size.
       * @param cvp - the cells viewport. Defines how many cells
       *              are visible.
       * @param pvp - the pixels viewport, allowing to compute the
       *              size of a tile on scree.
       * @param tileSize - the initial size of a tile.
       */
      IsometricFrame(const Viewport& cvp,
                     const Viewport& pvp,
                     const olc::vi2d& tileSize);

      ~IsometricFrame() = default;

      /**
       * @brief - Implementation of the interface method that
       *          define the conversion from tile coords to
       *          pixels. We apply an isometric projection to
       *          compute the corresponding pixels position.
       * @param x - the cell coordinate along the `x` axis.
       * @param y - the cell coordinate along the `y` axis.
       * @param pos - defines which position of the cell should
       *              be computed in pixels.
       * @return - the coordinates in pixels of the tile defined
       *           by the input coords.
       */
      olc::vf2d
      tileCoordsToPixels(float x,
                         float y,
                         const Cell& pos = Cell::CenterLeft) const noexcept override;

      /**
       * @brief - Implementation of the interface method to
       *          perform the reverse operation.
       * @param pixels - the pixels coordinates to convert into
       *                 tile coords.
       * @param intraTile - used to provide the intra tile coords
       *                    if a non `null` value is provided. The
       *                    coordinates are expressed in the range
       *                    `[0; 1]` (as in a percentage).
       * @return - the corresponding tile coordinates.
       */
      olc::vi2d
      pixelCoordsToTiles(const olc::vi2d& pixels,
                         olc::vf2d* intraTile = nullptr) const noexcept override;
  };

}

# include "IsometricFrame.hxx"

#endif    /* ISOMETRIC_FRAME_HH */

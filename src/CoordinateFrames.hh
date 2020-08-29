#ifndef    COORDINATE_FRAMES_HH
# define   COORDINATE_FRAMES_HH

# include "olcPixelGameEngine.h"

namespace new_frontiers {

  class CoordinateFrames {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param worldOrigin - the origin of the world in pixels. Allows
       *                      to represent the position of the origin
       *                      cell `(0, 0)` in screen coordinates. Used
       *                      internally for most of the conversions.
       * @param sprite - the size of the sprite in the source data file
       *                 in pixels.
       * @param tile - the size of a single cell on screen. It is the
       *               representation of a single sprite on screen. It
       *               is also used in most conversions.
       */
      CoordinateFrames(const olc::vi2d& worldOrigin,
                       const olc::vi2d& sprite = olc::vi2d(64, 64),
                       const olc::vi2d& tile = olc::vi2d(64, 32));

      ~CoordinateFrames() = default;

      /**
       * @brief - Returns the size of the sprite in the input image
       *          file. This size includes the potential whitespace
       *          included in the resource pack to accomodate for
       *          the ground textures.
       * @return - the sprite size in pixels.
       */
      const olc::vi2d&
      spriteSize() const noexcept;

      /**
       * @brief - Used to convert from sprite coordinates to the
       *          corresponding pixels coordinates. This method
       *          should mostly be used to locate a sprite in a
       *          resource pack.
       * @param x - the x coordinate of the sprite in the pack.
       * @param y - the y coordinate of the sprite in the pack.
       * @return - a vector representing the pixels coordinates
       *           for the input sprite coords.
       */
      olc::vi2d
      spriteCoordsToPixels(int x, int y) const noexcept;

      /**
       * @brief - Represent the size of a tile in the world as
       *          displayed on the screen. This size is at most
       *          equal to the sprite size and is usually a bit
       *          smaller because the sprite size include the
       *          characters which makes it higher.
       * @return - the tile size in pixels.
       */
      const olc::vi2d&
      tileSize() const noexcept;

      /**
       * @brief - Used to convert from tile coordinates to pixel
       *          coordinates. This method can be used when some
       *          tile is to be displayed on the screen. We make
       *          use of a global world origin that represents
       *          an offset applied to the world to make it more
       *          appealing and visible.
       * @param x - the cell coordinate along the `x` axis.
       * @param y - the cell coordinate along the `y` axis.
       * @return - the coordinates in pixels of the tile defined
       *           by the input coords.
       */
      olc::vi2d
      tileCoordsToPixels(int x, int y) const noexcept;

      /**
       * @brief - Convert from pixels coordinates to tile coords.
       *          Some extra logic is added in order to account
       *          for the tiles that do not align with the grid
       *          so that we always get an accurate position for
       *          the tile.
       * @param pixels - the pixels coordinates to convert into
       *                 tile coords.
       * @return - the corresponding tile coordinates.
       */
      olc::vi2d
      pixelCoordsToTiles(const olc::vi2d& pixels) const noexcept;

    private:

      /**
       * @brief - Defines the coordinates of the top part of the
       *          world in pixels' coordinates. Allows to offset
       *          the representation of the world on-screen and
       *          thus allow panning/zooming etc.
       */
      olc::vi2d m_wo;

      /**
       * @brief - Defines the size of the sprite in the original
       *          source file. Allows mainly to draw the correct
       *          piece of texture when representing an element.
       */
      olc::vi2d m_ss;

      /**
       * @brief - Defines the size of a tile on screen. This can
       *          be different from the sprite size as it's the
       *          adaptation of said sprite to the screen. It is
       *          also used to convert from cells' coordinates to
       *          screen coordinates and conversely.
       */
      olc::vi2d m_ts;
  };

}

# include "CoordinateFrames.hxx"

#endif    /* COORDINATE_FRAMES_HH */

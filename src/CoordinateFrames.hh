#ifndef    COORDINATE_FRAMES_HH
# define   COORDINATE_FRAMES_HH

# include "olcPixelGameEngine.h"

namespace new_frontiers {

  /**
   * @brief - Defines a viewport from its top left and
   *          bottom right corner. The zoom factor is
   *          implicit compared to the initial size of
   *          the tiles.
   *          The corners are expressed in cells.
   */
  struct Viewport {
    olc::vi2d tl;
    olc::vi2d br;
  };

  class CoordinateFrames {
    public:

      /**
       * @brief - Create a new coordinate frame handler.
       * @param worldOrigin - the origin of the world in pixels. Allows
       *                      to represent the position of the origin
       *                      cell `(0, 0)` in screen coordinates. Used
       *                      internally for most of the conversions.
       * @param tile - the size of a single cell on screen. It is the
       *               representation of a single sprite on screen. It
       *               is also used in most conversions.
       */
      CoordinateFrames(const olc::vi2d& worldOrigin,
                       const olc::vi2d& tile = olc::vi2d(64, 32));

      ~CoordinateFrames() = default;

      /**
       * @brief - Notify the coordinate frames to start a translation.
       *          This mainly concerns the world's origin: the goal is
       *          to fix the current world's origin so that it can be
       *          updated until the end of the translation.
       * @param origin - allows to keep track of the translation so
       *                 that we can define the transformation to be
       *                 applied to the world's origin.
       */
      void
      beginTranslation(const olc::vi2d& origin);

      /**
       * @brief - In case a translation has begun indicates that the
       *          new position to track is the input parameter. We
       *          will interpret it internally in regard to the value
       *          of the world origin and deduce the actual translation.
       * @param pos - the current final position of the translation.
       */
      void
      translate(const olc::vi2d& pos);

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
       * @brief - Defines the size of a tile on screen. This can
       *          be different from the sprite size as it's the
       *          adaptation of said sprite to the screen. It is
       *          also used to convert from cells' coordinates to
       *          screen coordinates and conversely.
       */
      olc::vi2d m_ts;

      /**
       * @brief - The origin of the translation (i.e. the pixels
       *          position when it started). Allows to compute
       *          the accumulated transform to apply to the wo.
       */
      olc::vi2d m_translationOrigin;

      /**
       * @brief - Cached position of the world's origin at the
       *          beginning of the translation. Used to apply
       *          the computed translation to update the world
       *          origin.
       */
      olc::vi2d m_cachedWo;
  };

}

# include "CoordinateFrames.hxx"

#endif    /* COORDINATE_FRAMES_HH */

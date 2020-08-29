#ifndef    COORDINATE_FRAMES_HH
# define   COORDINATE_FRAMES_HH

# include <core_utils/CoreObject.hh>
# include "olcPixelGameEngine.h"

namespace new_frontiers {

  /**
   * @brief - Defines a viewport from its top left
   *          corner and the associated dimensions.
   */
  struct Viewport {
    olc::vf2d p;
    olc::vf2d dims;
  };

  /**
   * @brief - Define the possible zoom types.
   */
  enum class Zoom {
    In,
    Out
  };

  class CoordinateFrames: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new coordinate frame handler with the input
       *          viewport and tiles size. The viewport will be used
       *          as a way to convert the pixels position to their
       *          cells counterpart.
       *          The size of a single cell in pixels is computed as
       *          a ratio between the viewport in cells and the one
       *          in pixels.
       * @param cViewport - the viewport expressed in world cells: it
       *                    define how many cells are visible in this
       *                    coordinate frame.
       * @param pViewport - the viewport expressed in pixels: it is
       *                    similar to the `cViewport` but defines
       *                    the screen visible area.
       * @param tileSize - defines the initial tile size as they are
       *                   described in the sprite file. This value
       *                   will be used to apply the scale ratio as
       *                   needed from the viewports.
       */
      CoordinateFrames(const Viewport& cViewport,
                       const Viewport& pViewport,
                       const olc::vi2d& tileSize);

      ~CoordinateFrames() = default;

      /**
       * @brief - Used to return the current scaling factor to apply
       *          to the tile compared to their initial size.
       * @return - the scaling factor to apply to the tiles.
       */
      const olc::vf2d&
      tileScale() const noexcept;

      /**
       * @brief - Notify the coordinate frames to start a translation.
       *          This mainly concerns the pixel viewport's origin:
       *          the goal is to fix the current top left corner so
       *          that it can be updated at the end of the translation.
       * @param origin - allows to keep track of the translation so
       *                 that we can define the transformation to be
       *                 applied to the pixel viewport.
       */
      void
      beginTranslation(const olc::vi2d& origin);

      /**
       * @brief - In case a translation has begun indicates that the
       *          new position to track is the input parameter. We
       *          will interpret it internally in regard to the value
       *          of the cached viewport origin and deduce the actual
       *          translation.
       * @param pos - the current final position of the translation.
       */
      void
      translate(const olc::vi2d& pos);

      /**
       * @brief - Used to perform the zoom operation requested. The
       *          viewports will be updated with a known factor so
       *          as to reflect the operation.
       *          The zoom operation happens by contracting or by
       *          expanding the viewport but we need a center: it
       *          should be provided by the user when calling the
       *          method and usually corresponds to the position of
       *          the mouse for maximum user-friendliness.
       * @param z - the zoom operation to perform.
       * @param pos - the position around which the center should
       *              be performed.
       */
      void
      zoom(const Zoom& z, const olc::vf2d& pos);

      /**
       * @brief - Used to convert from tile coordinates to pixel
       *          coordinates. This method can be used when some
       *          tile is to be displayed on the screen. We make
       *          use of a global position of the viewport to be
       *          able to correctly interpret the input cell. It
       *          is also dependent on the current scaling for
       *          tiles based on the cells viewport.
       * @param x - the cell coordinate along the `x` axis.
       * @param y - the cell coordinate along the `y` axis.
       * @return - the coordinates in pixels of the tile defined
       *           by the input coords.
       */
      olc::vf2d
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
      pixelCoordsToTiles(const olc::vi2d& pixels, int& q, olc::vi2d& gc, olc::vf2d& to) const noexcept;

    private:

      /**
       * @brief - Assume that the viewports are defined and use
       *          them to compute the scale of the tiles and the
       *          actual dimensions on screen from it and the
       *          original size of tiles.
       */
      void
      updateTileScale();

    private:

      /**
       * @brief - Define the viewport for this coordinate frame.
       *          It represent the area that is visible for now
       *          given the position of the camera.
       *          The viewport is expressed in cells and defined
       *          through its top left corner and dimensions.
       */
      Viewport m_cViewport;

      /**
       * @brief - Define a similar element but for the pixels on
       *          screen. It is used to represent the pixels that
       *          can be displayed on the device.
       *          Computing the ratio between both viewport gives
       *          the size of a tile in pixels and we can deduce
       *          a scaling factor from the initial size of a
       *          tile.
       */
      Viewport m_pViewport;

      /**
       * @brief - Define the initial size for a tile as described
       *          from the loaded sprite file. This size is used
       *          as a base to compute the scale from the values
       *          of the viewports.
       */
      olc::vi2d m_ts;

      /**
       * @brief - The current scale to apply to the tiles based
       *          on the values of the viewports. Depending on
       *          the zoom level and panning the tiles might get
       *          distorted and the factor is saved here.
       *          This value is changed when the viewports are
       *          modified.
       */
      olc::vf2d m_scale;

      /**
       * @brief - The size of the tiles once scaled: convenience
       *          value to cache the result of `m_ts * m_scale`
       *          as it is used quite often.
       */
      olc::vf2d m_tScaled;

      /**
       * @brief - The origin of the translation (i.e. the pixels
       *          position when it started). Allows to compute
       *          the accumulated transform to apply to the wo.
       */
      olc::vf2d m_translationOrigin;

      /**
       * @brief - Cached position of the top left corner of the
       *          cells viewport when starting the translation.
       *          Once the translation is performed we are able
       *          to update the viewport accordingly.
       */
      olc::vf2d m_cachedCOrigin;
  };

}

# include "CoordinateFrames.hxx"

#endif    /* COORDINATE_FRAMES_HH */

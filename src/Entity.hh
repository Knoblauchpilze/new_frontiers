#ifndef    ENTITY_HH
# define   ENTITY_HH

# include <memory>
# include "WorldElement.hh"
# include "VFX.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  /**
   * @brief - Convenience structure defining a path segment with
   *          its endpoints, a starting and an end time to allow
   *          for some sort of animation in between.
   */
  struct PathSegment {
    float xO, yO;
    float xT, yT;

    TimeStamp start;
    TimeStamp end;

    /**
     * @brief - Compute the coordinates along this path at the
     *          time defined by `moment`.
     * @param moment - the moment in time where this path is
     *                 evaluated.
     * @param x - the absicssa at this moment in time.
     * @param y - the ordinate at this moment in time.
     */
    void
    animate(const TimeStamp& moment, float& x, float& y) const;

    /**
     * @brief - Computes the length of this path in cells.
     * @return - the length of the path in cells.
     */
    float
    length() const noexcept;
  };

  /**
   * @brief - Describes the state of this entity: general
   *          info about its health and the effects that
   *          are applied to it are regrouped in here.
   */
  struct State {
    bool glowing;
    bool exhausted;
  };

  class Entity: public WorldElement<Mob> {
    public:

      /**
       * @brief - Creates a new entity with the specified tile
       *          and VFX produced.
       * @param tile - the visual representation of the entity
       *               along with its position.
       * @param vfx - the type of visual effects that the entity
       *              is able to produce.
       * @param vfxID - the VFX variant for this tile.
       * @param decaying - `true` if the VFX associated to this
       *                   entity is decaying.
       */
      Entity(const EntityTile& tile,
             const Effect& vfx,
             int vfxID = 0,
             bool decaying = true);

      /**
       * @brief - Implementation of the interface method to evolve
       *          this entity. It will mostly handle the motion of
       *          the entity and potential creation of VFX.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      virtual bool
      step(StepInfo& info);

      /**
       * @brief - Used to retrieve the current state of this entity.
       *          The state provide some useful indications about
       *          how the entity needs to be represented.
       * @return - the state of the entity.
       */
      const State&
      getState() const noexcept;

      /**
       * @brief - Returns the target abscissa of the path segment
       *          currently being traversed by the entity. In case
       *          no path is defined the entity's current position
       *          is returned.
       * @return - the abscissa of the current path segment.
       */
      float
      getPathX() const noexcept;

      /**
       * @brief - Similar to the `getPathX` but return the ordinate
       *          of the current path segment.
       * @return - the ordinate of the path segment.
       */
      float
      getPathY() const noexcept;

    protected:

      /**
       * @brief - Indicate that the entity should start glowing.
       * @param glowing - `true` if the entity is glowing.
       */
      void
      makeGlow(bool glowing) noexcept;

      /**
       * @brief - Indicate that the entity is now exhausted.
       * @param exhausted - `true` if the entity is exhausted.
       */
      void
      makeExhausted(bool exhausted) noexcept;

      /**
       * @brief - Used to create a new VFX tile at this entity's
       *          position.
       * @return - the VFX created from the internal parameters
       *           that this entity has about VFX.
       */
      VFXShPtr
      spawnVFX() const noexcept;

    private:

      /**
       * @brief - Used to select a path within admissible range of
       *          elements defined in the world.
       * @param info - a service to locate elements that might be
       *               blocking the path and various utilities.
       */
      void
      choosePath(const StepInfo& info);

    protected:

      /**
       * @brief - Defines the visual effect that can be produced
       *          by this entity in particular situations.
       */
      Effect m_vfx;

      /**
       * @brief - The variant of the VFX to produce.
       */
      int m_vfxID;

      /**
       * @brief - Whether or not the VFX associated to this entity
       *          is decaying.
       */
      bool m_vfxDecay;

      /**
       * @brief - Speed of the entity in cells per second.
       */
      float m_speed;

      /**
       * @brief - Defines the arrival radius for this entity when
       *          approaching its destination.
       */
      float m_rArrival;

      /**
       * @brief - The maximum length for a path selected by this
       *          entity. Expressed in cells.
       */
      float m_pathLength;

      /**
       * @brief - Boolean allowing to determine whether a path
       *          has already been selected.
       */
      bool m_hasPath;

      /**
       * @brief - The current path followed by this entity.
       */
      PathSegment m_path;

      /**
       * @brief - The current state of the entity.
       */
      State m_state;

    public:
      std::vector<float> m_cPoints;
  };

  using EntityShPtr = std::shared_ptr<Entity>;
}

# include "Entity.hxx"

#endif    /* ENTITY_HH */

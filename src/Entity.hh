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
       */
      Entity(const EntityTile& tile,
             const Effect& vfx);

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

    private:

      /**
       * @brief - Used to select a path within admissible range of
       *          elements defined in the world.
       * @param info - a service to locate elements that might be
       *               blocking the path and various utilities.
       */
      void
      choosePath(const StepInfo& info);

    private:

      /**
       * @brief - Defines the visual effect that can be produced
       *          by this entity in particular situations.
       */
      Effect m_vfx;

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
  };

  using EntityShPtr = std::shared_ptr<Entity>;
}

# include "Entity.hxx"

#endif    /* ENTITY_HH */

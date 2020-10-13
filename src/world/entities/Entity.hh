#ifndef    ENTITY_HH
# define   ENTITY_HH

# include <memory>
# include "Element.hh"
# include "../effects/Pheromon.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  /**
   * @brief - Convenience structure defining a path segment with
   *          its endpoints, a starting and a direction to easily
   *          make entities following the path move.
   */
  struct PathSegment {
    float xO, yO;
    float xT, yT;

    float xD, yD;

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

  class Entity: public Element<tiles::Entity> {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create an entity. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct Props {
        EntityTile tile;
        float radius;

        float perception;
        float health;

        float arrival;
        float pathLength;
      };

      /**
       * @brief - Creates a new entity with the specified tile
       *          to use for the visual representation.
       * @param props - the properties describing the entity
       *                to create.
       */
      Entity(const Props& props);

      /**
       * @brief - Implementation of the interface method to evolve
       *          this entity. It will mostly handle the motion of
       *          the entity and potential creation of VFX.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      bool
      step(StepInfo& info) override;

      /**
       * @brief - Used to retrieve the current state of this entity.
       *          The state provide some useful indications about
       *          how the entity needs to be represented.
       * @return - the state of the entity.
       */
      const State&
      getState() const noexcept;

      float
      getPerceptionRadius() const noexcept;

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

      /**
       * @brief - Return `true` in case this entity is already on
       *          a path to reach a target. We assume that this is
       *          the case if the distance to the target is larger
       *          than the distance to arrival.
       * @return - `true` if the entity is en route to a target.
       */
      bool
      isEnRoute() const noexcept;

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
      PheromonShPtr
      spawnPheromon(const pheromon::Type& type) const noexcept;

      /**
       * @brief - Interface method guaranteed to be called as
       *          the first action in the `step`method. It can
       *          be used by inheriting classes to perform
       *          some initialization of attributes if needed.
       * @param info - the information about the environment.
       */
      virtual void
      prepareForStep(const StepInfo& info) = 0;

      /**
       * @brief - Interface method to make the entity choose the
       *          next position it wants to travel to. This class
       *          handles the motion to the target but not the
       *          picking operation.
       * @param info - information about the world to help in the
       *               decision process.
       * @param x - the output abscissa of the chosen coordinate.
       * @param y - the output ordinate of the chosen coordinate.
       * @return - `true` if an action was taken.
       */
      virtual bool
      takeAction(StepInfo& info, float& x, float& y) = 0;

      /**
       * @brief - Interface method called after the execution of
       *          each `step` method. It can be used to make sure
       *          that any action has been performed.
       * @param info - the information about the environment.
       */
      virtual void
      postStep(StepInfo& info) = 0;

      /**
       * @brief - Used to make sure that the target of a path that
       *          would be started at the current entity's position
       *          and running `d` cells in the direction pointed at
       *          by `xDir` and `yDir` would stay in admissible
       *          bounds.
       *          Input arguments will be modified directly.
       * @param info - the information to use to clamp coordinates.
       * @param xDir - the direction along the `x` axis.
       * @param yDir - the direction along the `y` axis.
       * @param d - the distance for this path.
       */
      void
      normalizePath(const StepInfo& info, float& xDir, float& yDir, float& d) const noexcept;

    private:

      /**
       * @brief - Used to select a path within admissible range of
       *          elements defined in the world.
       * @param info - a service to locate elements that might be
       *               blocking the path and various utilities.
       */
      void
      choosePath(StepInfo& info);

    protected:

      /**
       * @brief - Speed of the entity in cells per second.
       */
      float m_speed;

      /**
       * @brief - Definition of the perception radius that
       *          this entity has. It is expressed in units
       *          of the world.
       *          We make sure that this value is at least
       *          a positive value.
       */
      float m_perceptionRadius;

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
       * @brief - The current path followed by this entity.
       */
      PathSegment m_path;

      /**
       * @brief - The current state of the entity.
       */
      State m_state;

      /**
       * @brief - Save of the duration that passed since the
       *          current path has been started. Allows to
       *          save the progression upon a pause event to
       *          restore it when a resume occurs.
       */
      Duration m_passed;

    public:
      std::vector<float> m_cPoints;
  };

  using EntityShPtr = std::shared_ptr<Entity>;
}

# include "Entity.hxx"

#endif    /* ENTITY_HH */

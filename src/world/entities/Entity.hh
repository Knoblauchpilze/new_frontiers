#ifndef    ENTITY_HH
# define   ENTITY_HH

# include <memory>
# include "Element.hh"
# include "../effects/Pheromon.hh"
# include "TimeUtils.hh"
# include "Path.hh"

namespace new_frontiers {

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

        utils::Uuid owner;

        /**
         * @brief - Used to make the compiler consider this
         *          structure as polymorphic and thus allow
         *          dynamic_cast operator on derived elems.
         */
        virtual ~Props();
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
       */
      void
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
       * @brief - Returns the current path followed by the entity.
       * @return - the current path followed by the entity.
       */
      path::Path
      getPath() const noexcept;

      /**
       * @brief - Convenience wrapper to determine whether this
       *          entity is still en route on its current path.
       * @return - `true` if the entity is still moving along
       *           its path.
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
       * @param path - the path to populate with the action. Note
       *               that this path is an input output argument
       *               which is initialized with no segment and
       *               the current position of the entity.
       * @return - `true` if an action was taken.
       */
      virtual bool
      takeAction(StepInfo& info, path::Path& path) = 0;

      /**
       * @brief - Interface method called after the execution of
       *          each `step` method. It can be used to make sure
       *          that any action has been performed.
       * @param info - the information about the environment.
       */
      virtual void
      postStep(StepInfo& info) = 0;

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
      path::Path m_path;

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
  };

  using EntityShPtr = std::shared_ptr<Entity>;
  using EntityPropsShPtr = std::shared_ptr<Entity::Props>;
}

# include "Entity.hxx"

#endif    /* ENTITY_HH */

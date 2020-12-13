#ifndef    WORLD_ELEMENT_HH
# define   WORLD_ELEMENT_HH

# include <core_utils/TimeUtils.hh>
# include <core_utils/CoreObject.hh>
# include <core_utils/Uuid.hh>

namespace new_frontiers {

  /// Forward declaration of the `StepInfo` to be used
  /// in the class definition.
  class StepInfo;

  class WorldElement: public utils::CoreObject {
    public:

      /**
       * @brief - Interrogate the internal identifier for the
       *          owner of this entity and return `true` if
       *          it is valid.
       * @return - `true` if the owner is valid.
       */
      bool
      isOwned() const noexcept;

      /**
       * @brief - Return a reference to the identifier of the
       *          owner of this element. Note that the uuid
       *          might be invalid if the `isOwned` returns
       *          `true`.
       * @return - the identifier of the owner of this elem.
       */
      const utils::Uuid&
      getOwner() const noexcept;

      /**
       * @brief - Interface method allowing for a world element
       *          to evolve based on its surroundings. We use a
       *          struct gathering the current state of the
       *          world to take decisions based on it.
       *          Inheriting classes can subclass this method
       *          to provide custom behavior.
       * @param info - all the information about the current
       *               state of the world.
       */
      virtual void
      step(StepInfo& info) = 0;

      /**
       * @brief - Interface method allowing to pause this world
       *          element: all internal behavior that depend on
       *          time should be stopped so that they can be
       *          later resumed.
       * @param t - the timestamp at which the pause occur.
       */
      virtual void
      pause(const utils::TimeStamp& t) = 0;

      /**
       * @brief - Opposite operation to the `pause`, this method
       *          is called to indicate that internal processes
       *          for this world element can be resumed.
       * @param t - the timestamp at which the resume occur.
       */
      virtual void
      resume(const utils::TimeStamp& t) = 0;

    protected:

      /**
       * @brief - Create a new world element with the specified
       *          name and owner.
       *          The owner can be left empty in order to make
       *          this element independant.
       * @param name - the name of this element.
       * @param owner - the owner of this element or an invalid
       *                identifier if the element is not owned.
       */
      WorldElement(const std::string& name,
                   const utils::Uuid& owner = utils::Uuid());

      /**
       * @brief - Used to define a new owner for this element.
       * @param uuid - the identifier of the new owner of the
       *               element.
       */
      void
      setOwner(const utils::Uuid& uuid);

    private:

      /**
       * @brief - The identifier of the owner of this element.
       *          It is used to make sure that elements can
       *          cooperate and identify each other in the
       *          simulation.
       */
      utils::Uuid m_owner;
  };

}

# include "WorldElement.hxx"

#endif    /* WORLD_ELEMENT_HH */

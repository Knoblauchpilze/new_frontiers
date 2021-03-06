#ifndef    COLONY_HH
# define   COLONY_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"
# include <maths_utils/Point2.hh>

namespace new_frontiers {

  namespace colony {

    /**
     * @brief - Defines the current focus of a colony in
     *          terms of behavior.
     */
    enum class Priority {
      Consolidation,  //< Specifies that the colony is
                      //< focusing on consolidating its
                      //< current position.
      Expansion,      //< Specifies that the colony is
                      //< giving priority to expanding
                      //< its territory even though the
                      //< new conquestes might no be
                      //< very well defended.
      War             //< Specifies that the colony is
                      //< focused on being prepared in
                      //< case a war occurs.
    };

    /**
     * @brief - Used to convert a string into a valid
     *          priority value. In case the string does
     *          not correspond to any known focus type
     *          a `Consolidation` value is returned and
     *          the error flag is set to `true`.
     * @param str - the string to convert.
     * @param error - `true` if the string could not be
     *                decoded.
     * @return - the corresponding focus.
     */
    Priority
    strToFocus(const std::string& str, bool& error) noexcept;

  }

  class Colony: public WorldElement {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a colony. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct Props {
        utils::Uuid id;

        utils::Point2f home;

        float budget;
        float actionCost;
        float refill;

        colony::Priority focus;

        float radius;
        int maxSize;

        unsigned warThreshold;
      };

      /**
       * @brief - Create a new colony with the provided uuid.
       *          The identifier has to be valid otherwise an
       *          exception is thrown. The colony will not yet
       *          have any elements spawned in game.
       *          The position provided in input locates the
       *          colony in the world and gives an indication
       *          of its preferential location.
       * @param props - the properties defining the colony to
       *                create.
       */
      Colony(const Props& props);

      /**
       * @brief - Destruction of the colony. Used to make sure
       *          that each element belonging to the colony is
       *          correctly marked for self-destruction.
       */
      ~Colony();

      /**
       * @brief - Return a number in the range `[0; 1]` that
       *          indicates how far from taking a new action
       *          this colony is. `0` would mean that it has
       *          probably just taken an action while `1` is
       *          an indication that it will probably take a
       *          new action very soon.
       * @return - the ratio indicating how far the thought
       *           process is situated.
       */
      float
      getActionRatio() const noexcept;

      colony::Priority
      getFocus() const noexcept;

      /**
       * @brief - Define the new focus of the colony. Will be
       *          replacing the current focus upon the next
       *          `step` invocation.
       * @param focus - the focus of the colony.
       */
      void
      setFocus(const colony::Priority& focus);

      /**
       * @brief - Return `true` if the colony is currently
       *          active. The active status is a way to be
       *          able to select a particular colony among
       *          a list. It is mostly used to be able to
       *          control the colony by external means and
       *          recognize it as somehow activated.
       * @return - `true` if the colony is set to active.
       */
      bool
      isActive() const noexcept;

      /**
       * @brief - Define whether this colony is active.
       * @param active - the active status for this colony.
       */
      void
      setActive(bool active) noexcept;

      /**
       * @brief - Implementation of the interface methdo to
       *          make this `Colony` evolve.
       * @param info - all the information about the current
       *               state of the world.
       */
      void
      step(StepInfo& info) override;

      /**
       * @brief - Implementation of the interface method to
       *          make this `Colony` pause internal processes.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const utils::TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to
       *          make this `Colony` resume its activity.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const utils::TimeStamp& t) override;

    private:

      /**
       * @brief - Used internally at the beginning of the
       *          `step` function in order to perform the
       *          actualization of internal time dependend
       *          props for this colony.
       *          Allows to nicely group all the processing
       *          in a single method.
       * @param info - information about the current time
       *               in the world.
       */
      void
      update(const StepInfo& info);

      /**
       * @brief - Main thinking process for the colony. It
       *          attempts to change the focus to something
       *          more appropriate based on the surroundings
       *          of the colony at the moment of calling the
       *          method.
       *          Verification to make sure that the budget
       *          for thinking is not exhausted is performed.
       * @param info - information about the world.
       */
      void
      changeFocus(const StepInfo& info);

      /**
       * @brief - Used to perform the thinking process of
       *          the colony based on its current focus.
       *          This means spawning new blocks as needed
       *          and performing adjustments to bring the
       *          colony to a better state given its focus.
       * @param info - information about the world.
       */
      void
      think(StepInfo& info);

    private:

      /**
       * @brief - The preferred position for this colony. It
       *          will usually mean that the activity for this
       *          colony will be around this position (at least
       *          for direct actions of the colony).
       */
      utils::Point2f m_home;

      /**
       * @brief - The current focus of the colony. Starts with
       *          a value and updated along the life of the
       *          colony based on the perceptions.
       */
      colony::Priority m_focus;

      /**
       * @brief - The current resource budget for this colony.
       *          It corresponds to the available thinking
       *          power available. Each action consumes a cost
       *          defined by `m_actionCost` so the larger this
       *          value the more actions can be taken.
       */
      float m_budget;

      /**
       * @brief - The cost taking a single action for the colony.
       *          It goes from spawning a new portal to changing
       *          other priorities.
       */
      float m_actionCost;

      /**
       * @brief - Base refill rate for this colony. Defines how
       *          fast the `m_budget` resource is refilling over
       *          time. This resource is the base used to create
       *          new spawners and more generally take action as
       *          a colony. A higher refill rate indicates a
       *          more active colony (as more actions can be
       *          taken).
       */
      float m_refill;

      /**
       * @brief - Define the territory that corresponds to the
       *          colony. It is used to materialize the space
       *          that this colony consider home and is allowed
       *          to use to spawn elements and change in terms
       *          of properties.
       *          This will be the region where blocks spawned
       *          by the colony will be located.
       */
      float m_radius;

      /**
       * @brief - Define the maximum number of spawners that
       *          can be created by the colony at any given
       *          time. It allows to control the growth of a
       *          well fed colony so that it does not become
       *          too big.
       */
      int m_maxSize;

      /**
       * @brief - Used to determine the current size of the
       *          colony in terms of blocks spawned. This is
       *          used in association with the max size val
       *          to make sure that we don't grow too big.
       */
      int m_size;

      /**
       * @brief - A boolean indicating whether or not the colony
       *          is considered active. Active colonies do not
       *          have a particular behavior, it is more a way
       *          to mark this colony so that external processes
       *          can know that it should be treated differently.
       *          This is typically used in a context where the
       *          user can change the behavior of the colony if
       *          needed (like changing the focus, gifting some
       *          energy, etc.) in a list of colonies.
       *          Any colony starts as inactive.
       */
      bool m_active;

      /**
       * @brief - Defines a threshold of enemy entities above
       *          which the entity will enter the war mode.
       *          As soon as too many entities have entered
       *          the reserved space for this colony we will
       *          try to take action by producing warriors
       *          and thus fighting the invaders.
       */
      unsigned m_peaceToWarThreshold;
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

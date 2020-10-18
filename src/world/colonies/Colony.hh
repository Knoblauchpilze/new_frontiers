#ifndef    COLONY_HH
# define   COLONY_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"

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

        float homeX;
        float homeY;

        float budget;
        float actionCost;
        float refill;

        colony::Priority focus;
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
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to
       *          make this `Colony` resume its activity.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

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
       * @brief - The preferred abscissa for this colony. It
       *          will usually mean that the activity for this
       *          colony will be around this position (at least
       *          for direct actions of the colony).
       */
      float m_homeX;

      /**
       * @brief - Similar to the `m_homeX` but handles the
       *          preferred ordinate for the colony.
       */
      float m_homeY;

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
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

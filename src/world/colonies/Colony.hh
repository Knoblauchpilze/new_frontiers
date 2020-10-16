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
       * @brief - Create a new colony with the provided uuid.
       *          The identifier has to be valid otherwise an
       *          exception is thrown. The colony will not yet
       *          have any elements spawned in game.
       *          The position provided in input locates the
       *          colony in the world and gives an indication
       *          of its preferential location.
       * @param uuid - the identifier of the colony.
       * @param x - the abscissa of the preferred position of
       *            this colony.
       * @param y - the ordinate of the preferred position of
       *            this colony.
       */
      Colony(const utils::Uuid& uuid,
             float x,
             float y);

      /**
       * @brief - Destruction of the colony. Used to make sure
       *          that each element belonging to the colony is
       *          correctly marked for self-destruction.
       */
      ~Colony();

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
       */
      float m_budget;
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

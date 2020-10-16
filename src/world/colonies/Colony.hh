#ifndef    COLONY_HH
# define   COLONY_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"

namespace new_frontiers {

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
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

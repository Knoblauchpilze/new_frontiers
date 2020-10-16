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
       * @param uuid - the identifier of the colony.
       */
      Colony(const utils::Uuid& uuid);

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
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

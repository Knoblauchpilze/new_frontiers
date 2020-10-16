#ifndef    COLONY_HH
# define   COLONY_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include <core_utils/CoreObject.hh>

namespace new_frontiers {

  class Colony: public utils::CoreObject {
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

    private:

      /**
       * @brief - The identifier of this colony. Allows to know
       *          which elements of the simulation belong to the
       *          colony. It also help individual entities to be
       *          aware of whether other entities are from the
       *          same colony or not.
       *          This value is assigned once and for all in the
       *          life of the colony.
       */
      utils::Uuid m_uuid;
  };

  using ColonyShPtr = std::shared_ptr<Colony>;
}

# include "Colony.hxx"

#endif    /* COLONY_HH */

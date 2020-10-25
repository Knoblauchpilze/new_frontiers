#ifndef    ENTITY_ACTION_HH
# define   ENTITY_ACTION_HH

# include <memory>
# include "Action.hh"

namespace new_frontiers {

  class EntityAction: public Action {
    public:

      /**
       * @brief - Create a new action allowing to perform some
       *          modification of an entity. Typical use cases
       *          include spawning or removing an entity from
       *          the world.
       * @param type - the type of this action.
       * @param ent - the tile of the entity this action is
       *                related to.
       * @param variant - the variant of the entity.
       */
      EntityAction(const action::Type& type,
                   const tiles::Entity& ent,
                   int variant = 0);

      /**
       * @brief - Desctruction of the object.
       */
      virtual ~EntityAction();

      /**
       * @brief - Implementation of the effect this action has
       *          on the input world. This typically results in
       *          spawning a new entity or removing one.
       * @param w - the world onto which the action should be
       *            applied.
       */
      void
      apply(World& w) const override;

    private:

      /**
       * @brief - The tile of the entity attached to this action.
       */
      tiles::Entity m_entity;

      /**
       * @brief - The variant of the entity attached to this
       *          action.
       */
      int m_eVariant;
  };

  using EntityActionShPtr = std::shared_ptr<EntityAction>;
}

# include "EntityAction.hxx"

#endif    /* ENTITY_ACTION_HH */

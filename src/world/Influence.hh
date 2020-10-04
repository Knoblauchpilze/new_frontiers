#ifndef    INFLUENCE_HH
# define   INFLUENCE_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "Element.hh"

namespace new_frontiers {

  // Forward declaration to be able to declare variables for
  // the step info class.
  class Block;
  class Entity;
  class VFX;

  using BlockShPtr = std::shared_ptr<Block>;
  using EntityShPtr = std::shared_ptr<Entity>;
  using VFXShPtr = std::shared_ptr<VFX>;

  namespace influence {

    /**
     * @brief - Enumeration defining the possible types
     *          of influences that can be produced by
     *          an in-game element.
     */
    enum class Type {
      BlockSpawn,
      BlockRemoval,
      EntitySpawn,
      EntityRemoval,
      VFXSpawn,
      VFXRemoval
    };

  }

  class Influence: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param block - the block onto which the influence will
       *                apply.
       */
      Influence(const influence::Type& type,
                Block* block);

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param block - the block onto which the influence will
       *                apply.
       */
      Influence(const influence::Type& type,
                BlockShPtr block);

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param entity - the entity onto which the influence will
       *                 apply.
       */
      Influence(const influence::Type& type,
                Entity* entity);

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param entity - the entity onto which the influence will
       *                 apply.
       */
      Influence(const influence::Type& type,
                EntityShPtr entity);

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param vfx - the vfx onto which the influence will apply.
       */
      Influence(const influence::Type& type,
                VFX* vfx);

      /**
       * @brief - Create a new influence with the specified type.
       * @param type - the type of the influence to create.
       * @param vfx - the vfx onto which the influence will apply.
       */
      Influence(const influence::Type& type,
                VFXShPtr vfx);

      ~Influence() = default;

      const influence::Type&
      getType() const noexcept;

      /**
       * @brief - Return the block associated to this influence
       *          in case it is not `null`. If it is an error
       *          is raised.
       * @return - the pointer to the block associated to this
       *           influence.
       */
      Block*
      getBlock() const;

      /**
       * @brief - Similar to the `getBlock` method but returns
       *          the shared pointer on a block. Also raised an
       *          error in case this value is `null`.
       * @return - the pointer to the block associated to this
       *           influence.
       */
      BlockShPtr
      getShPBlock() const;

      /**
       * @brief - Similar to `getBlock` but return the entity that
       *          is linked this influence.
       * @return - the entity associated to this influence.
       */
      Entity*
      getEntity() const;

      /**
       * @brief - Similar to `getBlock` but return the entity that
       *          is linked this influence.
       * @return - the entity associated to this influence.
       **/
      EntityShPtr
      getShPEntity() const;

      /**
       * @brief - Similar to `getBlock` but return the vfx that
       *          is linked this influence.
       * @return - the vfx associated to this influence.
       */
      VFX*
      getVFX() const;

      /**
       * @brief - Similar to `getBlock` but return the vfx that
       *          is linked this influence.
       * @return - the vfx associated to this influence.
       **/
      VFXShPtr
      getShPVFX() const;

    private:

      /**
       * @brief - The type of the influence. Allows to determine
       *          how the related block should be interpreted.
       */
      influence::Type m_type;

      /**
       * @brief - One possible receiver of the influence. It is
       *          only used if the type refers to an action of
       *          deleting a block. This allows the block to
       *          mark itself for deletion without duplicating
       *          memory management through a shared pointer.
       */
      Block* m_block;

      /**
       * @brief - Similar to the `m_block` but used in case of
       *          a creation influence. This ensures that the
       *          memory is still safely managed even if this
       *          block is not yet handled by the `World`.
       */
      BlockShPtr m_shPBlock;

      /**
       * @brief - Similar to the `m_block` but receives an entity
       *          pointer to apply the influence.
       */
      Entity* m_entity;

      /**
       * @brief - Used in a similar way to `m_shPBlock`.
       */
      EntityShPtr m_shPEntity;

      /**
       * @brief - Similar to the `m_block` but for a VFX.
       */
      VFX* m_vfx;

      /**
       * @brief - Used in a similar way to `m_shPBlock`.
       */
      VFXShPtr m_shPVFX;
  };

  using InfluenceShPtr = std::shared_ptr<Influence>;
}

# include "Influence.hxx"

#endif    /* INFLUENCE_HH */

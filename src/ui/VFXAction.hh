#ifndef    VFX_ACTION_HH
# define   VFX_ACTION_HH

# include <memory>
# include "Action.hh"

namespace new_frontiers {

  class VFXAction: public Action {
    public:

      /**
       * @brief - Create a new action allowing to perform some
       *          modification of a VFX. Typical use cases are
       *          spawning or removing an VFX from the world.
       * @param type - the type of this action.
       * @param vfx - the tile of the VFX this action is
       *                related to.
       * @param variant - the variant of the VFX.
       */
      VFXAction(const action::Type& type,
                const tiles::Effect& vfx,
                int variant = 0);

      /**
       * @brief - Desctruction of the object.
       */
      virtual ~VFXAction();

      /**
       * @brief - Implementation of the effect this action has
       *          on the input world. This typically results in
       *          spawning a new VFX or removing one.
       * @param w - the world onto which the action should be
       *            applied.
       */
      void
      apply(World& w) const override;

    private:

      /**
       * @brief - The tile of the VFX attached to this action.
       */
      tiles::Effect m_vfx;

      /**
       * @brief - The variant of the VFX attached to this
       *          action.
       */
      int m_vVariant;
  };

  using VFXActionShPtr = std::shared_ptr<VFXAction>;
}

# include "VFXAction.hxx"

#endif    /* VFX_ACTION_HH */

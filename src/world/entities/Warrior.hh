#ifndef    WARRIOR_HH
# define   WARRIOR_HH

# include "Mob.hh"

namespace new_frontiers {

  class Warrior: public Mob {
    public:

      /**
       * @brief - Creates a new mob with the specified tile.
       * @param tile - the visual representation of the entity
       *               along with its position.
       * @param homeX - the abscissa of the home position for
       *                this mob.
       * @param homeY - the abscissa of the home position for
       *                this mob.
       */
      Warrior(const EntityTile& tile,
              float homeX,
              float homeY);

    protected:

      bool
      chase(StepInfo& info, float& x, float& y) override;

      bool
      fight(StepInfo& info, float& x, float& y) override;

      bool
      getBack(StepInfo& info, float& x, float& y) override;

      bool
      wander(StepInfo& info, float& x, float& y) override;

    private:
  };

  using WarriorShPtr = std::shared_ptr<Warrior>;
}

# include "Warrior.hxx"

#endif    /* WARRIOR_HH */

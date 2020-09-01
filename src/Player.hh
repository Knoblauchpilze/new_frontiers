#ifndef    PLAYER_HH
# define   PLAYER_HH

# include <memory>
# include "Entity.hh"

namespace new_frontiers {

  class Player: public Entity {
    public:

      /**
       * @brief - Creates a new player at the specified location.
       * @param x - the abscissa of the player in cells.
       * @param y - the ordinate of the player in cells.
       */
      Player(float x, float y);

      /**
       * @brief - Override the base class implementation to be
       *          receptive to the user's input.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      virtual bool
      step(StepInfo& info);

    private:

      /**
       * @brief - The sprint speed of the player. Greater than
       *          the actual speed but can only be maintained
       *          for a short period of time.
       */
      float m_sprintSpeed;

      /**
       * @brief - The speed when the player is recovering. It
       *          is smaller than the standard speed in order
       *          to make the user think before sprinting.
       */
      float m_recoverySpeed;

      /**
       * @brief - The duration the player can sprint before
       *          being exhausted.
       */
      Duration m_exhaustion;

      /**
       * @brief - The duration needed for the player to recover
       *          the strength to sprint again.
       */
      Duration m_recovery;

      /**
       * @brief - A time stamp used to meausre the beginning
       *          of the current phase for the player, be it
       *          a recovery or a sprinting phase.
       */
      TimeStamp m_origin;
  };

  using PlayerShPtr = std::shared_ptr<Player>;
}

# include "Player.hxx"

#endif    /* PLAYER_HH */

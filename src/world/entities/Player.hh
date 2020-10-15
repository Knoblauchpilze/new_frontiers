#ifndef    PLAYER_HH
# define   PLAYER_HH

# include <memory>
# include "Entity.hh"

namespace new_frontiers {

  class Player: public Entity {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a player. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct PProps: Props {
        float sprintSpeed;
        float recoverySpeed;

        Duration exhaustion;
        Duration recovery;
      };

      /**
       * @brief - Creates a new player at the specified location.
       * @param props - the properties describing the player.
       */
      Player(const PProps& props);

      /**
       * @brief - Override the base class implementation to be
       *          receptive to the user's input.
       * @param info - the information about the world to use for
       *               the entity to adapt its behavior.
       * @return - `true` if the entity has moved (and thus should
       *           be repainted).
       */
      bool
      step(StepInfo& info) override;

      /**
       * @brief - Implementation of the interface method to pause
       *          the internal processes for this player. It mostly
       *          include freezing the state of the player.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to resume
       *          the internal processes for this player. Resuming
       *          the state to a valid configuration is involved.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const TimeStamp& t) override;

    protected:

      /**
       * @brief - Dummy implementation of this method: the player
       *          does not have anything to prepare for.
       * @param info - the information about the world.
       */
      void
      prepareForStep(const StepInfo& info) override;

      /**
       * @brief - Dummy implementation of the method as the player is
       *          controlled by the user and not automatically.
       * @param info - info to take the decision of the next target.
       * @param x - output argument representing the abscissa of the
       *            chosen location.
       * @param y - output argument representing the ordinate of the
       *            chosen location.
       * @return - `true` if an action was taken.
       */
      bool
      takeAction(StepInfo& info, float& x, float& y) override;

      /**
       * @brief - Dummy implementation of the post step method.
       * @param info - data about the world's of this entity.
       */
      void
      postStep(StepInfo& info) override;

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

      /**
       * @brief - A duration that represents the remaining
       *          duration for any effect that this player
       *          might be submitted to when a pause event
       *          occurs.
       */
      Duration m_remaining;
  };

  using PlayerShPtr = std::shared_ptr<Player>;
}

# include "Player.hxx"

#endif    /* PLAYER_HH */

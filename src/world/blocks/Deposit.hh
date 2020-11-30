#ifndef    DEPOSIT_HH
# define   DEPOSIT_HH

# include <memory>
# include "Block.hh"

namespace new_frontiers {

  class Deposit: public Block {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a deposit. It is meant
       *          as a way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct DProps: Props {
        float stock;
      };

      /**
       * @brief - Create a new deposit that contains some amount
       *          of a resource. Each deposit comes with a certain
       *          amount of resource and can be refilled with the
       *          appropriate method.
       * @param props - the properties describing this deposit.
       */
      Deposit(const DProps& props);

      /**
       * @brief - Used to retrieve the current stock for this
       *          deposit.
       * @return - the current stock in this deposit.
       */
      float
      getStock() const noexcept;

      /**
       * @brief - Used to change the amount of the resource
       *          contained by this deposit by the specified
       *          amount.
       *          In case the amount is too large to be removed
       *          from the internal stock, the operation is not
       *          performed unless the `force` boolean says so.
       *          We just guarantee that the stock does not go
       *          below `0`.
       * @param delta - the amount of resource to add or deplete.
       * @param force - `true` if the refill operation should
       *                always be successful.
       * @return - the amount that was actually removed or added
       *           from/to the deposit.
       */
      float
      refill(float delta, bool force = false);

    private:

      /**
       * @brief - The amount of resource contains in this
       *          deposit.
       */
      float m_stock;

      friend class BlockFactory;
  };

  using DepositShPtr = std::shared_ptr<Deposit>;
}

# include "Deposit.hxx"

#endif    /* DEPOSIT_HH */

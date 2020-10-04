#ifndef    DEPOSIT_HXX
# define   DEPOSIT_HXX

# include "Deposit.hh"

namespace new_frontiers {

  inline
  bool
  Deposit::refill(float delta, bool force) {
    // In case the `delta` is positive the operation
    // always succeed.
    if (delta >= 0.0f) {
      m_stock += delta;

      return true;
    }

    // Otherwise we have to have a sufficient amount
    // of resource to perform the operation. In case
    // the `force` is `true` we will decrease the
    // stock no matter if it becomes negative. If we
    // don't force the update then we only make the
    // stock `0`.
    if (m_stock < delta && force) {
      m_stock -= delta;

      return false;
    }

    m_stock = std::max(m_stock - delta, 0.0f);

    return true;
  }


  using DepositShPtr = std::shared_ptr<Deposit>;
}

#endif    /* DEPOSIT_HXX */

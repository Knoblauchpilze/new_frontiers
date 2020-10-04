
# include "Deposit.hh"

namespace new_frontiers {

  Deposit::Deposit(const BlockTile& tile, float stock):
    Block(tile, "deposit", 1.0f),

    m_stock(stock)
  {}

}

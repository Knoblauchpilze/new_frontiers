
# include "Deposit.hh"

namespace new_frontiers {

  Deposit::Deposit(const DProps& props):
    Block(props, "deposit"),

    m_stock(props.stock)
  {}

}

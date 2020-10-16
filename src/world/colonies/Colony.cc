
# include "Colony.hh"

namespace new_frontiers {

  Colony::Colony(const utils::Uuid& uuid,
                 float x,
                 float y):
    WorldElement(uuid.toString(), uuid),

    m_homeX(x),
    m_homeY(y)
  {
    setService("colony");

    if (!uuid.valid()) {
      error(
        "Unable to create colony from uuid",
        "Invalid uuid"
      );
    }
  }

}

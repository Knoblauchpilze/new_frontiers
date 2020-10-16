
# include "Colony.hh"

namespace new_frontiers {

  Colony::Colony(const utils::Uuid& uuid):
    utils::CoreObject(uuid.toString()),

    m_uuid(uuid)
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

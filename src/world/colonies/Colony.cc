
# include "Colony.hh"

namespace new_frontiers {

  Colony::Colony(const utils::Uuid& uuid):
    WorldElement(uuid.toString(), uuid)
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

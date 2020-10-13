
# include "Block.hh"

namespace new_frontiers {

  Block::Block(const Props& props,
               const std::string& name):
    Element(props.tile, props.radius, props.health, name)
  {}

}

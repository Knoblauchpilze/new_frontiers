
# include "StepInfo.hh"

namespace new_frontiers {

  void
  StepInfo::spawnBlock(BlockShPtr e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::BlockSpawn, e));
  }

  void
  StepInfo::removeBlock(Block* e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::BlockRemoval, e));
  }

  void
  StepInfo::spawnEntity(EntityShPtr e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::EntitySpawn, e));
  }

  void
  StepInfo::removeEntity(Entity* e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::EntityRemoval, e));
  }

  void
  StepInfo::spawnVFX(VFXShPtr e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::VFXSpawn, e));
  }

  void
  StepInfo::removeVFX(VFX* e) {
    influences.push_back(std::make_shared<Influence>(influence::Type::VFXRemoval, e));
  }

}

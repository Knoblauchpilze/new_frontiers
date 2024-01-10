#ifndef    WORLD_HXX
# define   WORLD_HXX

# include "World.hh"
# include "blocks/BlockFactory.hh"
# include "effects/PheromonFactory.hh"

namespace new_frontiers {

  inline
  World::~World() {}

  inline
  unsigned
  World::w() const noexcept {
    return m_w;
  }

  inline
  unsigned
  World::h() const noexcept {
    return m_h;
  }

  inline
  LocatorShPtr
  World::locator() const noexcept {
    return m_loc;
  }

  inline
  void
  World::setBlockProps(BlockPropsShPtr props) {
    m_actions.block = props;
    m_actions.type = ActionType::Block;

    // Consistency check.
    if (m_actions.block == nullptr) {
      warn("Assigned invalid null block action");
      m_actions.type = ActionType::None;
    }
  }

  inline
  void
  World::setEntityProps(EntityPropsShPtr props) {
    m_actions.ent = props;
    m_actions.type = ActionType::Entity;

    // Consistency check.
    if (m_actions.ent == nullptr) {
      warn("Assigned invalid null entity action");
      m_actions.type = ActionType::None;
    }
  }

  inline
  void
  World::setVFXProps(VFXPropsShPtr props) {
    m_actions.vfx = props;
    m_actions.type = ActionType::VFX;

    // Consistency check.
    if (m_actions.vfx == nullptr) {
      warn("Assigned invalid null vfx action");
      m_actions.type = ActionType::None;
    }

    if (m_actions.type != ActionType::None) {
      Pheromon::PProps* pp = dynamic_cast<Pheromon::PProps*>(m_actions.vfx.get());
      if (pp == nullptr) {
        warn("Unhandled vfx action which is not a pheromon");

        m_actions.vfx = nullptr;
        m_actions.type = ActionType::None;
      }
    }
  }

  inline
  void
  World::switchToNextOwner() {
    // In case no owner is yet defined, pick
    // the first one (if possible).
    if (m_colonies.empty()) {
      m_actions.owner.invalidate();
    }

    if (!m_actions.owner.valid()) {
      m_actions.owner = m_colonies[0u]->getOwner();
      m_colonies[0u]->setActive(true);
      return;
    }

    unsigned id = 0u;
    while (id < m_colonies.size() && m_colonies[id]->getOwner() != m_actions.owner) {
      ++id;
    }

    // Check whether we could find out which
    // colony is currently active.
    if (id >= m_colonies.size()) {
      warn(
        "Could not find colony corresponding to current active one " +
        m_actions.owner.toString() + ", switching to first one"
      );

      m_actions.owner = m_colonies[0u]->getOwner();
      m_colonies[0u]->setActive(true);
      return;
    }

    m_colonies[id]->setActive(false);
    m_actions.owner = m_colonies[(id + 1u) % m_colonies.size()]->getOwner();
    m_colonies[(id + 1u) % m_colonies.size()]->setActive(true);
  }

  inline
  void
  World::initializeActions() {
    // Initialize empty actions.
    m_actions.block = nullptr;
    m_actions.vfx = nullptr;
    m_actions.ent = nullptr;

    // By default no action is selected.
    m_actions.type = ActionType::None;

    // No owner of any of the items that
    // can be spawned.
    m_actions.owner.invalidate();
  }

  inline
  void
  World::generate() {
    // Generate elements.
    generateElements();

    // Create the locator service from the
    // elements of this world.
    m_loc = std::make_shared<Locator>(m_w, m_h, m_blocks, m_entities, m_vfx, m_colonies);
  }

  inline
  void
  World::loadDimensions(std::ifstream& in) {
    // Read each dimension: should be on separate lines.
    in >> m_w;
    if (in.eof()) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions")
      );
    }
    in >> m_h;

    if (m_w <= 0 || m_h <= 0) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions ") + std::to_string(m_w) +
        "x" + std::to_string(m_h)
      );
    }
  }

}

#endif    /* WORLD_HXX */

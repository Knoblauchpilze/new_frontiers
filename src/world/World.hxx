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
  World::setDepositProps(const Deposit::DProps& props) {
    m_actions.deposit = props;
    m_actions.type = ActionType::Block;
  }

  inline
  void
  World::setPheromonProps(const Pheromon::PProps& props) {
    m_actions.pheromon = props;
    m_actions.type = ActionType::VFX;
  }

  inline
  void
  World::initializeActions() {
    // Deposit properties.
    m_actions.deposit = BlockFactory::newDepositProps(0.0f, 0.0f);

    // Pheromon properties.
    m_actions.pheromon = PheromonFactory::newPheromonProps(0.0f, 0.0f, tiles::Fire);
    m_actions.pheromon.type = pheromon::Type::Collect;

    m_actions.pheromon.radius = 0.5f;

    // By default no action is selected.
    m_actions.type = ActionType::None;
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

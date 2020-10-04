#ifndef    INFLUENCE_HXX
# define   INFLUENCE_HXX

# include "Influence.hh"

namespace new_frontiers {

  inline
  Influence::Influence(const influence::Type& type,
                       Block* block):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(block),
    m_shPBlock(nullptr),
    m_entity(nullptr),
    m_shPEntity(nullptr),
    m_vfx(nullptr),
    m_shPVFX(nullptr)
  {
    // Make sure there's an element.
    if (m_block == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null block receiver object")
      );
    }
  }

  inline
  Influence::Influence(const influence::Type& type,
                       BlockShPtr block):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(nullptr),
    m_shPBlock(block),
    m_entity(nullptr),
    m_shPEntity(nullptr),
    m_vfx(nullptr),
    m_shPVFX(nullptr)
  {
    // Make sure there's an element.
    if (m_shPBlock == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null block receiver object")
      );
    }
  }

  inline
  Influence::Influence(const influence::Type& type,
                       Entity* entity):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(nullptr),
    m_shPBlock(nullptr),
    m_entity(entity),
    m_shPEntity(nullptr),
    m_vfx(nullptr),
    m_shPVFX(nullptr)
  {
    // Make sure there's an element.
    if (m_entity == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null entity receiver object")
      );
    }
  }

  inline
  Influence::Influence(const influence::Type& type,
                       EntityShPtr entity):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(nullptr),
    m_shPBlock(nullptr),
    m_entity(nullptr),
    m_shPEntity(entity),
    m_vfx(nullptr),
    m_shPVFX(nullptr)
  {
    // Make sure there's an element.
    if (m_shPEntity == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null entity receiver object")
      );
    }
  }

  inline
  Influence::Influence(const influence::Type& type,
                       VFX* vfx):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(nullptr),
    m_shPBlock(nullptr),
    m_entity(nullptr),
    m_shPEntity(nullptr),
    m_vfx(vfx),
    m_shPVFX(nullptr)
  {
    // Make sure there's an element.
    if (m_vfx == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null vfx receiver object")
      );
    }
  }

  inline
  Influence::Influence(const influence::Type& type,
                       VFXShPtr vfx):
    utils::CoreObject("influence"),

    m_type(type),

    m_block(nullptr),
    m_shPBlock(nullptr),
    m_entity(nullptr),
    m_shPEntity(nullptr),
    m_vfx(nullptr),
    m_shPVFX(vfx)
  {
    // Make sure there's an element.
    if (m_shPVFX == nullptr) {
      error(
        std::string("Could not create influence"),
        std::string("Invalid null vfx receiver object")
      );
    }
  }

  inline
  const influence::Type&
  Influence::getType() const noexcept {
    return m_type;
  }

  inline
  Block*
  Influence::getBlock() const {
    if (m_block == nullptr) {
      error(
        std::string("Cannot retrieve block for influence"),
        std::string("No block associated to influence")
      );
    }

    return m_block;
  }

  inline
  BlockShPtr
  Influence::getShPBlock() const {
    if (m_shPBlock == nullptr) {
      error(
        std::string("Cannot retrieve block for influence"),
        std::string("No block associated to influence")
      );
    }

    return m_shPBlock;
  }

  inline
  Entity*
  Influence::getEntity() const {
    if (m_entity == nullptr) {
      error(
        std::string("Cannot retrieve entity for influence"),
        std::string("No entity associated to influence")
      );
    }

    return m_entity;
  }

  inline
  EntityShPtr
  Influence::getShPEntity() const {
    if (m_shPEntity == nullptr) {
      error(
        std::string("Cannot retrieve entity for influence"),
        std::string("No entity associated to influence")
      );
    }

    return m_shPEntity;
  }

  inline
  VFX*
  Influence::getVFX() const {
    if (m_vfx == nullptr) {
      error(
        std::string("Cannot retrieve vfx for influence"),
        std::string("No vfx associated to influence")
      );
    }

    return m_vfx;
  }

  inline
  VFXShPtr
  Influence::getShPVFX() const {
    if (m_shPVFX == nullptr) {
      error(
        std::string("Cannot retrieve vfx for influence"),
        std::string("No vfx associated to influence")
      );
    }

    return m_shPVFX;
  }

}

#endif    /* INFLUENCE_HXX */

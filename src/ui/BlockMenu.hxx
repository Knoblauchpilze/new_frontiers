#ifndef    BLOCK_MENU_HXX
# define   BLOCK_MENU_HXX

# include "BlockMenu.hh"
# include "BlockAction.hh"

namespace new_frontiers {

  BlockMenu::BlockMenu(const olc::vi2d& pos,
                       const olc::vf2d& size,
                       const std::string& name,
                       const action::Type& type,
                       BlockPropsShPtr block,
                       Menu* parent):
    Menu(pos, size, name, Layout::Horizontal, parent),

    m_block(block),
    m_type(type)
  {
    // Check the action's type.
    if (m_type != action::Type::Creation && m_type != action::Type::Deletion) {
      error(
        "Unable to create block menu",
        "Invalid action type " + std::to_string(static_cast<int>(m_type)) +
        " but expected " + std::to_string(static_cast<int>(action::Type::Creation)) + " or " +
        std::to_string(static_cast<int>(action::Type::Deletion))
      );
    }

    // Check the block's description.
    if (m_block == nullptr) {
      error(
        "Unable to create block menu",
        "Invalid null block properties description"
      );
    }
  }

  inline
  BlockMenu::~BlockMenu() {}

  inline
  void
  BlockMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Push a new action with the corresponding block
    // and type to be executed.
    actions.push_back(std::make_shared<BlockAction>(m_type, m_block));
  }

}

#endif    /* BLOCK_MENU_HXX */

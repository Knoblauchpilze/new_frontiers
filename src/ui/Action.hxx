#ifndef    ACTION_HXX
# define   ACTION_HXX

# include "Action.hh"

namespace new_frontiers {

  inline
  Action::Action(const std::string& name):
    utils::CoreObject(name)
  {}

  inline
  Action::~Action() {}

}

#endif    /* ACTION_HXX */

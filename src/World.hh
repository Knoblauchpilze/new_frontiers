#ifndef    WORLD_HH
# define   WORLD_HH

# include <core_utils/CoreObject.hh>

namespace new_frontiers {

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new world with the specified dimensions.
       * @param width - the width of the world in cells.
       * @param height - the height of the window in cells.
       */
      World(unsigned width = 640u,
            unsigned height = 480u);

      /**
       * @brief - Desctruction of the object.
       */
      ~World();

      unsigned
      w() const noexcept;

      unsigned
      h() const noexcept;

    private:

    private:
    
      /**
       * @brief - Dimensions of the world in cells.
       */
      unsigned m_w;
      unsigned m_h;
  };

}

# include "World.hxx"

#endif    /* WORLD_HH */

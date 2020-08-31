#ifndef    SOLID_ELEMENT_HH
# define   SOLID_ELEMENT_HH

# include <memory>
# include "WorldElement.hh"

namespace new_frontiers {

  class SolidElement: public WorldElement<Sprite> {
    public:

      /**
       * @brief - Base implementation for an element which
       *          does nothing.
       * @param info - information about the world.
       * @return - `false` as nothing happens here.
       */
      bool
      step(StepInfo& info) override;

    protected:

      /**
       * @brief - Create a new solid element with the tile
       *          and name. Only used to forward the args
       *          to the base class.
       * @param tile - the visual representation of this
       *               element along with its position.
       * @param name - the name of the object.
       */
      SolidElement(const SolidTile& tile,
                   const std::string& name);
  };

  using SolidElementShPtr = std::shared_ptr<SolidElement>;
}

# include "SolidElement.hxx"

#endif    /* SOLID_ELEMENT_HH */

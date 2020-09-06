#ifndef    PHEROMON_HH
# define   PHEROMON_HH

# include <memory>
# include "VFX.hh"
# include "TimeUtils.hh"

namespace new_frontiers {

  namespace pheromon {

    /**
     * @brief - Define the possible type of pheromon that
     *          can be laid out by entities. Each one has
     *          a particular role and is interpreted in a
     *          different way by entities.
     */
    enum class Type {
      Wander,
      Chase,
      Fight,
      Return
    };

  }

  class Pheromon: public VFX {
    protected:

      /**
       * @brief - Create a new pheromon with the specified
       *          type and visual representation.
       * @param type - the type of this pheromon, which is
       *               some sort of indication of its role.
       * @param vfx - the visual representation for this
       *              pheromon. It will be passed on to the
       *              base class constructor.
       * @param decay - the decay duration for this pheromon.
       */
      Pheromon(const pheromon::Type& type,
               const VFXTile& vfx,
               const std::vector<Duration>& decay);

    private:

      friend class PheromonFactory;

      /**
       * @brief - The type of this pĥeromon. It is linked to
       *          the reason why this pheromon was laid out
       *          in the first place and allow entities to
       *          adapt their behavior when such a pheromon
       *          is perceived.
       */
      pheromon::Type m_type;
  };

  using PheromonShPtr = std::shared_ptr<Pheromon>;
}

# include "Pheromon.hxx"

#endif    /* PHEROMON_HH */

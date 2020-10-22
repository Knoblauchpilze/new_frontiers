#ifndef    PHEROMON_HH
# define   PHEROMON_HH

# include <memory>
# include "TimeUtils.hh"
# include "EvaporatingVFX.hh"

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
      Collect,
      Fight,
      Return
    };

  }

  class Pheromon: public EvaporatingVFX {
    public:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create an VFX. It is meant as a
       *          way to reduce the number of arguments
       *          provided to the constructor of this class.
       */
      struct PProps: EProps {
        pheromon::Type type;
      };

      /**
       * @brief - Create a new pheromon with the specified
       *          type and visual representation.
       * @param props - the description of the properties
       *                of this pheromon.
       */
      Pheromon(const PProps& props);

      const pheromon::Type&
      getType() const noexcept;

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

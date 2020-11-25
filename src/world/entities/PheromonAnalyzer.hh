#ifndef    PHEROMON_ANALYZER_HH
# define   PHEROMON_ANALYZER_HH

# include <unordered_map>
# include <core_utils/CoreObject.hh>
# include "../effects/Pheromon.hh"

namespace new_frontiers {

  class PheromonAnalyzer: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new pheromon analyzer. This object
       *          is able to accumulate information about the
       *          pheromons surrounding an entity which can be
       *          used to weigh the relative importance of
       *          various pheromons in the decision making
       *          process.
       * @param props - the properties describing this mob.
       */
      PheromonAnalyzer();

      /**
       * @brief - Define a new weight for the pheromon associated
       *          to the input type. The weights associated to the
       *          other type of pheromones won't be changed so it
       *          might lead to total weight larger than `1`.
       * @param type - the type of the pheromon for which a weight
       *               is specified.
       * @param w - the weight to assign to this pheromons.
       */
      void
      setRelativeWeight(const pheromon::Type& type, float w);

      /**
       * @brief - In addition to the pheromons, this analyzer is
       *          able to include some measure of random in the
       *          decision process. This method allows to specify
       *          how much of it should be integrated.
       * @param w - the weight of randomness in the decision making
       *            process.
       */
      void
      setRandomWeight(float w);

      /**
       * @brief - Accumulate the input pheromon with its position
       *          and amount. The relevant internal attributes are
       *          updated to register this pheromon.
       * @param p - the pheromon to accumulate.
       */
      void
      accumulate(const Pheromon& p);

      /**
       * @brief - Used to take a decision of a target based on the
       *          pheromons currently accumulated and the weights
       *          to give to each kind.
       * @param x - output value which will hold the abscissa of
       *            the target. The input value *must* represent
       *            a random target that will be used in case a
       *            non zero weight is assigned to the randomness
       *            for this analyzer.
       * @param y - output value which will hold the ordinate of
       *            the target. The input value *must* represent
       *            a random target that will be used in case a
       *            non zero weight is assigned to the randomness
       *            for this analyzer.
       */
      void
      computeTarget(float& x, float& y);

    private:

      /**
       * @brief - Convenience structure allowing to represent
       *          the known information about a pheromon. We
       *          compute the position and the weights for
       *          each one along with the number of elements
       *          that compose the information.
       */
      struct PheromonInfo {
        float x;
        float y;
        float w;
        int count;

        void
        accumulate(float locX, float locY, float amount);

        void
        normalize();
      };

      /**
       * @brief - Initialize the relative weights of pheromons
       *          to default values.
       */
      void
      initialize();

    private:

      /**
       * @brief - Convenience using to define iterators.
       */
      using CInfoIt = std::unordered_map<pheromon::Type, PheromonInfo>::const_iterator;
      using InfoIt = std::unordered_map<pheromon::Type, PheromonInfo>::iterator;
      using CWIt = std::unordered_map<pheromon::Type, float>::const_iterator;
      using WIt = std::unordered_map<pheromon::Type, float>::iterator;

      /**
       *  @brief - Convenience define to refer to the key
       *           representing the information about a
       *           pheromon in the below maps.
       */
      using PIKey = std::pair<const pheromon::Type, PheromonInfo>;
      using WKey = std::pair<const pheromon::Type, float>;

      /**
       * @brief - A map defining the relative weights to apply
       *          to each pheromon type when taking a decision.
       *          This allows to adapt the importance of each
       *          type of external stimulus as needed.
       */
      std::unordered_map<pheromon::Type, float> m_weights;

      /**
       * @brief - The relative weight of randomness in the
       *          decision making process. This is similar
       *          to any weight in the `m_weights` table
       *          but it accounts for pure randomness.
       */
      float m_rndWeight;

      /**
       * @brief - Accumulated information about pheromons in
       *          the surroundings of the element attached to
       *          this analyzer.
       */
      std::unordered_map<pheromon::Type, PheromonInfo> m_info;
  };

}

# include "PheromonAnalyzer.hxx"

#endif    /* PHEROMON_ANALYZER_HH */

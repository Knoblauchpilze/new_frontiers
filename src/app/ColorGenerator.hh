#ifndef    COLOR_GENERATOR_HH
# define   COLOR_GENERATOR_HH

# include <vector>
# include <string>
# include <unordered_map>
# include <unordered_set>
# include "olcPixelGameEngine.h"

namespace new_frontiers {

  class ColorGenerator {
  public:

    /**
     * @brief - Builds a new color generator with an empty
     *          color map allowing to generate colors in a
     *          somewhat random manner.
     *          There is no guarantee that the palette will
     *          be actually nice nor very distinct but all
     *          colors will be different.
     * @param tries - the number of tries before declaring
     *                a failure when generating a new color.
     * @param preventBlack - `true` if the color black is
     *                       valid and `false` otherwise.
     */
    ColorGenerator(unsigned tries = 10u,
                   bool preventBlack = true);

    ~ColorGenerator() = default;

    /**
     * @brief - Attempt to create a new color under with the
     *          specified key. In case the key has already
     *          been encountered the generated color will be
     *          returned.
     *          Otherwise a new color will be generated with
     *          that is guaranteed to be different from all
     *          colors previously generated.
     *          In case a color cannot be generated in a
     *          reasonnable amount of tries (which can be
     *          configured) an error is reported.
     * @param key - a key describing the client for which the
     *              color should be generated. This allows to
     *              access to a previously generated color by
     *              specifying again the same key.
     * @param failed - output boolean indicating whether the
     *                 color could be generated in less than
     *                 the number of tries configured.
     * @return - the color associated to the input name (be it
     *           generated or retrieved from the cache).
     */
    olc::Pixel
    generate(const std::string& key, bool& failed);

  private:

    static
    olc::Pixel
    toColor(unsigned color) noexcept;

    static
    unsigned
    toUint(unsigned r, unsigned g, unsigned b) noexcept;

  private:

    /**
     * @brief - Convenience define for a map of colors.
     */
    using Colors = std::unordered_map<std::string, unsigned>;

    /**
     * @brief - Used to keep a list of the registered colors
     *          in the `Colors` map. Allows to search by colors
     *          and not by name.
     */
    using Cache = std::unordered_set<unsigned>;

    /**
     * @brief - An integer value indicating how many tries
     *          should be made when creating a color before
     *          considering that generating a new color is
     *          not possible.
     *          The larger this value the longer it might
     *          take to generate a new color.
     */
    unsigned m_tries;

    /**
     * @brief - Used to prevent black color from being
     *          generated.
     */
    bool m_preventBlack;

    /**
     * @brief - The list of colors already registered,
     *          sorted by key. Note that the value
     *          corresponds to a `marshalled` version
     *          of the color where each channel is
     *          stored alongside. Convenience functions
     *          are provided to de/serialize the colors
     *          from this unique value.
     */
    Colors m_colors;

    /**
     * @brief - The list of keys registered in the
     *          `m_colors` array. This is used in
     *          order to be able to search by colors
     *          rather than by key.
     */
    Cache m_existing;
  };

}

# include "ColorGenerator.hxx"

#endif    /* COLOR_GENERARTOR_HH */

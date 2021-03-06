#ifndef    UTILS_HH
# define   UTILS_HH

# include <string>
# include "olcPixelGameEngine.h"

template <typename CoordinateType>
std::string
toString(const olc::v2d_generic<CoordinateType>& vec) noexcept;

template <typename CoordinateType>
std::ostream&
operator<<(std::ostream& out, const olc::v2d_generic<CoordinateType>& vec) noexcept;

template <typename CoordinateType>
std::ostream&
operator<<(const olc::v2d_generic<CoordinateType>& vec, std::ostream& out) noexcept;

std::string
toString(const olc::Pixel& p) noexcept;

# include "utils.hxx"

#endif    /* UTILS_HH */

#ifndef    APP_DESC_HXX
# define   APP_DESC_HXX

# include "AppDesc.hh"

namespace new_frontiers {

  inline
  AppDesc
  newDesc(const olc::vi2d& dims,
          CoordinateFrameShPtr cf,
          const std::string& name) noexcept
  {
    return AppDesc{
      dims,
      olc::vi2d(1, 1),

      name,
      cf
    };
  }

}

#endif    /* APP_DESC_HXX */

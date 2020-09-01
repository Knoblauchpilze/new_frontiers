project (new_frontiers_lib)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set (SOURCES
  olcEngine.cc
  NewFrontiersApp.cc
  World.cc
  RNG.cc
  WorldIterators.cc
  CoordinateFrames.cc
  SolidElement.cc
  Spawner.cc
  Entity.cc
  VFX.cc
  Teleporter.cc
  WorldElementLocator.cc
  Player.cc
  HostileMob.cc
  )

add_library (new_frontiers_lib SHARED
  ${SOURCES}
  )

set(NEW_FRONTIERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_link_libraries(new_frontiers_lib
  sdl_core
  png
  X11
  GL
  pthread
  stdc++fs
  )

target_include_directories (new_frontiers_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

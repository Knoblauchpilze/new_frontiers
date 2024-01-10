project (new_frontiers_lib)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/world
  )
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/ui
  )
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/coordinates
  )
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/app
  )

set (SOURCES
  ${SOURCES}
  olcEngine.cc
  PGEApp.cc
  )

add_library (new_frontiers_lib SHARED
  ${SOURCES}
  )

set (NEW_FRONTIERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_link_libraries(new_frontiers_lib
  core_utils
  png
  X11
  GL
  pthread
  stdc++fs
  )

target_include_directories (new_frontiers_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/world
  )

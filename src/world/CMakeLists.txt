
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/blocks
  )
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/effects
  )
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/entities
  )

set (SOURCES
  ${SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/World.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Iterators.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Locator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/StepInfo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/RNG.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Influence.cc
  PARENT_SCOPE
  )


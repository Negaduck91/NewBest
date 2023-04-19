cmake_minimum_required(VERSION 3.0.0)
project(NewBest VERSION 0.1.0)


include_directories("${CMAKE_SOURCE_DIR}")

include(CTest)
enable_testing()

add_executable(NewBest main.cpp Menus.cxx Logger.cxx Utilities.cxx BaseScaff.cxx)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

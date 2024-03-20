cmake_minimum_required (VERSION 3.25)

project(DiscCheckEmu)

include(cmake/CPM.cmake)

add_subdirectory (DiscCheckEmuAPIHook)
add_subdirectory (DiscCheckEmuLoader)


cmake_minimum_required (VERSION 3.8)

project ("DiscCheckEmu")

include("CPM.cmake")

add_subdirectory ("DiscCheckEmuAPIHook")
add_subdirectory ("DiscCheckEmuLoader")


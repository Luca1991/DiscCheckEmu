project (DCEAPIHook) 

include(../cmake/Detours.cmake)

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")

add_library (${PROJECT_NAME} SHARED
"APIHook.cpp"
"Config/ConfigParser.cpp"
"Res/Resources.rc"
"Util/StringUtils.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE detours yaml-cpp winmm.lib)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
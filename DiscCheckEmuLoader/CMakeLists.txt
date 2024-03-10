project (DCELoader) 

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")

add_executable (${PROJECT_NAME} WIN32
"DCELoader.cpp"
"ConfigParser.cpp"
"Injector.cpp"
"Res/Resources.rc"
)

target_sources(${PROJECT_NAME} PRIVATE "Res/DCELoader.manifest")

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
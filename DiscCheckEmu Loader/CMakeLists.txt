add_executable (DCELoader WIN32
"DCELoader.cpp"
"ConfigParser.h"
"ConfigParser.cpp"
"Injector.h"
"Injector.cpp"
"Version.h"
"Res/Resources.rc")

target_sources(DCELoader PRIVATE "Res/DCELoader.manifest")

target_link_libraries(DCELoader PUBLIC yaml-cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DCELoader PROPERTY CXX_STANDARD 20)
endif()

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")


add_library (DCEAPIHook SHARED "APIHook.cpp"
"APIHook.h"
"ConfigParser.cpp"
"ConfigParser.h"
"APIConfig/GetDriveAConfig.h"
"APIConfig/GetVolumeInformationAConfig.h"
"Hook/GetDriveTypeA.h"
"Hook/GetVolumeInformationA.h"
"Version.h"
"Res/Resources.rc" "Hook/CreateFileA.h" "Hook/GetFileAttributesA.h" "Util/StringUtils.h" "Util/StringUtils.cpp" "Hook/mciSendCommand.h" "APIConfig/MciSendCommandConfig.h")


target_link_libraries(DCEAPIHook PUBLIC detours yaml-cpp winmm.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DCEAPIHook PROPERTY CXX_STANDARD 20)
endif()

# Detours library setup. Inspired from:
# https://github.com/LandSandBoat/xiloader/blob/main/cmake/detours.cmake
CPMAddPackage(
    NAME detours
    GITHUB_REPOSITORY microsoft/Detours
    VERSION 4.0.1
    DOWNLOAD_ONLY YES)
if (detours_ADDED)
  # Detours has no CMake support, so we create our own target
  # https://github.com/0xeb/detours-cmake

    # Copy from <detours>/src to <detours>/detours
    file(COPY ${detours_SOURCE_DIR}/src/ DESTINATION ${detours_SOURCE_DIR}/detours/)

    file(GLOB detours_sources ${detours_SOURCE_DIR}/detours/*.cpp)

    # This file is included and not compiled on its own
    list(REMOVE_ITEM detours_sources "${detours_SOURCE_DIR}/detours/uimports.cpp")

    add_library(detours STATIC ${detours_sources})

    target_compile_options(detours PRIVATE /W4 /WX /Zi /Gy /Gm- /Zl)
    target_include_directories(detours
      PUBLIC
        $<BUILD_INTERFACE:${detours_SOURCE_DIR}>
    )
endif()

CPMAddPackage("gh:jbeder/yaml-cpp#0.8.0")
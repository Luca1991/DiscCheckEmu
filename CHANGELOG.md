# DiscCheckEmu Changelog


## [0.6.0] - 2024-06-07

### Added
- Added phenoProtect to the list of supported protections
- GetFileAttributesW and FindFirstFileW hooks. Automatically setup when using file/directory redirections. This is to support some games that use QuickTime videos.
- stringToWidestring function in MemoryUtils
- Added Roxxe to the list of supported protections

### Changed
- Improved GetVolumeInformationA logging

### Fixed
- Directory redirection: fixed various bugs


## [0.5.0] - 2024-05-18

### Added
- Added Cheats support
- Added directory redirection support


## [0.4.1] - 2024-05-03

### Changed
- Updated spdlog to v1.14.1
- MemoryUtils and Patch small optimizations

### Changed
- Patches: now using rva instad of virtual address


## [0.4.0] - 2024-04-24

### Added
- Memory Patches support
- MemoryUtils

### Changed
- Reorganized project structure


## [0.3.0] - 2024-04-12

### Added
- RegEnumValueA hook
- Ability to pass parameters to the process to be created

### Changed
- API Logger (only in Debug build) is now powered by spdlog.
- Optimized hooking engine install/uninstall. 
- Optimized injector code for better performance.
- Now using /WX flag in both Debug and Release builds.


## [0.2.1] - 2024-03-22

### Added
- "/WX" flag in debug builds

### Fixed
- RegQueryValueExA hook: fixed various bugs + code optimizations


## [0.2.0] - 2024-03-20

### Added
- FindFirstFileA hook (currently only used in file redirection)
- RegQueryValueExA hook
- DCE is now compatible with BitPool DRM!

### Changed
- Added more info on debuggers into ConfigurationDocumentation.md (thanks to @greenozon).

### Fixed
- Fixed some warnings (thanks to @greenozon).


## [0.1.1] - 2024-03-07

### Fixed
- Crash in Debug build (API Logging)


## [0.1.0] - 2024-03-07

### Added
- GetFileAttributesA hook
- GetDiskFreeSpaceA Hook
- The debug version now features a debug prompt with logs of each intercepted API

### Changed
- Refactored code for better readability.
- Various code optimizations.
- Improved project structure.

### Fixed
- Bug in GetVolumeInformationA hook


## [0.0.1] - 2024-02-19

### Added
- Initial release.

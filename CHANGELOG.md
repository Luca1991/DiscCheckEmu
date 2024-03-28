# DiscCheckEmu Changelog


## [next] - Unreleased

### Added
- RegEnumValueA hook

### Changed
- API Logger (only in Debug build) is now powered by spdlog.


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

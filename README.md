# DiscCheckEmu

DiscCheckEmu is a tool designed to emulate disc checks found in many disc-based games and software without the need
for physical media. By intercepting API calls and modifying their behavior on the fly, DiscCheckEmu provides a seamless
experience for users looking to enjoy their favorite legacy software without the hassle of keeping CDs or ISOs mounted.

As computers increasingly lack CD-ROM drives, accessing old disc-based games and software has become impractical.
DiscCheckEmu addresses this issue by allowing users to emulate disc checks, enabling them to run their favorite titles
directly from their hard drives, without physically patching the game executable.

Please note that DiscCheckEmu works only for games and software that employs standard disc checks and/or passive copy protections: old "active" disc-based protections
like SafeDisc, SecuROM or LaserLock are not (and will not be) supported by this software. This is due to the nature that these
protections that use segment encryption and IAT obfuscation. This is a list of copy protections that are known to be successfully handled by DCE:

* [engine32](https://github.com/TheRogueArchivist/DRML/blob/main/entries/engine32/engine32.md)
* [Bitpool](https://github.com/TheRogueArchivist/DRML/blob/main/entries/Bitpool/Bitpool.md)


## DCEConfigs Repository

If you are just looking for users pre-made DCEConfig file, go to: https://github.com/Luca1991/DCEConfigs

## Features

* Intercept API calls and modify their behavior without patching the executable.
* File redirection.
* Virtual Drive definition.
* API Logger (only available in Debug builds).
* Utilize a generic API hooking engine configurable through YAML files.
* Easy to use injector for automatically creating the target process and injecting the DLL.
* Fully open-source and customizable to suit individual needs.

## Components

* **DCELoader**: Main executable responsible for creating target process and injecting the DCEAPIHook DLL.
* **DCEAPIHook**: DLL used to hook APIs and modify their behavior.
* **DCEConfig**: YAML configuration file tailored to specific software, used by both DCELoader and DCEAPIHook.

## About DCEConfig

DCEConfig serves as the blueprint for configuring both DCELoader and DCEAPIHook. This file must be customized by
the user for each target software, detailing API hooking and process creation parameters. Refer to the
ConfigurationDocumentation file for assistance in writing a functional configuration file.

## Building

This project utilizes CMake. To build it, install Visual Studio 2022 with C++ packages and load the project 
as a CMake solution. Once loaded, press F7 to build the solution.

## How to use

Download the latest release from the download section and extract the archive.

Copy DCELoader.exe, DCEAPIHook.dll and DCEConfig.yaml to your game/software installation directory.

Edit DCEConfig.yaml writing a configuration for your target, then run DCELoader.exe and enjoy.

## Acknowledgements

* Microsoft: For developing the Detours library used in this project.
* jbeder: For developing the yaml-cpp library used in this project.
* gabime: For developing the spdlog library used in this project.


<div align="center">
  <img width="45%" src="https://raw.githubusercontent.com/Luca1991/DiscCheckEmu/main/dce_logo.png"/>
</div>
<br>
<div align="center">
  <img src="https://img.shields.io/github/v/release/Luca1991/DiscCheckEmu">
  <img src="https://img.shields.io/github/release-date/Luca1991/DiscCheckEmu">
    <img src="https://img.shields.io/github/downloads/Luca1991/DiscCheckEmu/total">
  <img src="https://img.shields.io/github/license/Luca1991/DiscCheckEmu">
</div>

# DiscCheckEmu

DiscCheckEmu is a tool designed to emulate disc checks found in many disc-based games and software without the need
for physical media. By intercepting API calls and modifying their behavior on the fly, DiscCheckEmu provides a seamless
experience for users looking to enjoy their favorite legacy software without the hassle of keeping CDs or ISOs mounted.

As computers increasingly lack CD-ROM drives, accessing old disc-based games and software has become impractical.
DiscCheckEmu addresses this issue by allowing users to emulate disc checks, enabling them to run their favorite titles
directly from their hard drives, without physically patching the game executable.

Please note that DiscCheckEmu works only for games and software that employs standard disc checks and/or passive copy protections.
Old "active" disc-based protections like SafeDisc or SecuROM are not directly supported by this software. This is due to the nature of these
protections using segment encryption and IAT obfuscation.

This is a list of copy protections that are known to be successfully handled by DCE:

* [engine32](https://github.com/TheRogueArchivist/DRML/blob/main/entries/engine32/engine32.md)
* [Bitpool](https://github.com/TheRogueArchivist/DRML/blob/main/entries/Bitpool/Bitpool.md)


> [!IMPORTANT]
> DiscCheckEmu is *NOT* intended for piracy: you must own the original game/software disc to legally use this tool!

## DCEConfigs Repository

If you are just looking for DCEConfig files created by other users, go to: https://github.com/Luca1991/DCEConfigs

## Features

* Intercept API calls and modify their behavior without patching the executable.
* File redirection.
* Virtual Drive definition.
* In-Memory Patcher.
* API Logger (only available in Debug builds).
* Utilize a generic API hooking engine configurable through YAML files.
* Easy to use injector for automatically creating the target process and injecting the DLL.
* Fully open-source and customizable to suit individual needs.

## Components

* **DCELoader**: Main executable responsible for creating target process and injecting the DCEAPIHook DLL.
* **DCEAPIHook**: DLL used to hook APIs and modify their behavior.
* **DCEConfig**: YAML configuration file tailored to specific software, used by both DCELoader and DCEAPIHook. DCEConfig serves as the blueprint for configuring both DCELoader and DCEAPIHook. This file must be customized by the user for each target software, detailing API hooking and process creation parameters. Refer to the [ConfigurationDocumentation](https://raw.githubusercontent.com/Luca1991/DiscCheckEmu/main/ConfigurationDocumentation.md) file for assistance in writing a functional configuration file.

## Building

This project utilizes CMake. To build it, install Visual Studio 2022 with C++ packages and load the project 
as a CMake solution. Once loaded, press F7 to build the solution.

## How to use

1. Download the latest release from the [download section](https://github.com/Luca1991/DiscCheckEmu/releases) and extract the archive.

2. Copy DCELoader.exe, DCEAPIHook.dll and DCEConfig.yaml to your game/software installation directory.

3. Edit DCEConfig.yaml writing a configuration for your target, then run DCELoader.exe and enjoy.

If you are receiving the **"Missing MSVCP140.dll"** error, make sure to download and install the latest x86 [Microsoft Visual C++ Redistributables](https://aka.ms/vs/17/release/vc_redist.x86.exe).

## Acknowledgements

* [Microsoft](https://github.com/microsoft): For developing the [Detours](https://github.com/microsoft/Detours) library used in this project.
* [jbeder](https://github.com/jbeder): For developing the [yaml-cpp](https://github.com/jbeder/yaml-cpp) library used in this project.
* [gabime](https://github.com/gabime): For developing the [spdlog](https://github.com/gabime/spdlog) library used in this project.

## Projects Using DiscCheckEmu

Here are some open-source projects that are using DiscCheckEmu:

**[SafeDiscLoader](https://github.com/nckstwrt/SafeDiscLoader)**: Allows playing SafeDisc (version 2.7 to the latest 4.9) protected games on modern Windows. SafeDiscLoader utilizes DCEAPIHook.dll to emulate additional disc checks found in many SafeDisc-protected games.


/*
	Copyright 2024 Luca D'Amico

	This file is part of DiscCheckEmu.

	DiscCheckEmu is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	DiscCheckEmu is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with DiscCheckEmu.  If not, see <http://www.gnu.org/licenses/>
*/

#pragma once


#define VERSION_MAJOR				0
#define VERSION_MINOR				1
#define VERSION_BUILD				0
#define VERSION_PRIVATE				0

#define VER_ORIGINAL_FILENAME		"DCEAPIHook.dll"
#define VER_PRODUCT_NAME			"DiscCheckEmu API Hook"
#define VER_FILE_DESCRIPTION		"API Hook component for DiscCheckEmu"
#define VER_COPYRIGHT_STR			"Copyright (C) 2024 Luca D'Amico. All Rights Reserved."

#ifndef NDEBUG
#define VER_FILEFLAGS				0x00000001L // VS_FF_DEBUG
#else
#define VER_FILEFLAGS				0
#endif

#define VER_FILEOS					0x00040004L // VOS_NT_WINDOWS32
#define VER_FILETYPE				0x00000002L // VFT_DLL

#define TOSTRING2(s)				#s
#define TOSTRING(s)					TOSTRING2(s)

#define FILE_VERSION				VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_PRIVATE
#define FILE_VERSION_STR			TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "." TOSTRING(VERSION_BUILD) "." TOSTRING(VERSION_PRIVATE)
#define PRODUCT_VERSION				FILE_VERSION
#define PRODUCT_VERSION_STR			FILE_VERSION_STR

#define IDI_ICON1					1

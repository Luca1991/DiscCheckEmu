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

#include <Windows.h>
#include "..\ApiHook.h"
#include "..\Util\StringUtils.h"

static HANDLE(WINAPI* OGCreateFileA)(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile) = CreateFileA;

HANDLE WINAPI HookedCreateFileA(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	SPDLOG_INFO("---> CreateFileA({0:s}, 0x{1:x}, 0x{2:x}, {3}, 0x{4:x}, 0x{5:x}, {6})",
		lpFileName, dwDesiredAccess, dwShareMode, "lpSecurityAttributes",
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	std::string newFileName(lpFileName);

	auto it = apiConfig.fileRedirections.find(string_utils::toLowercase(lpFileName));
	if (it != apiConfig.fileRedirections.end())
		newFileName = it->second;
	else
	{
		for (const auto& [key, value] : apiConfig.directoryRedirections)
		{
			if (string_utils::startsWithIgnoreCase(lpFileName, key))
			{
				newFileName = value + (lpFileName + key.size());
				// NOTE: We don't break here because we want to check if there are multiple directory (subdirectory) redirections
			}
		}
	}

	SPDLOG_INFO("<--- CreateFileA({0:s}, 0x{1:x}, 0x{2:x}, {3}, 0x{4:x}, 0x{5:x}, {6})",
		LPCSTR(newFileName.c_str()), dwDesiredAccess, dwShareMode, "lpSecurityAttributes",
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	return OGCreateFileA(LPCSTR(newFileName.c_str()), dwDesiredAccess, dwShareMode,
		lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);
}
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

static HANDLE(WINAPI* OGFindFirstFileA)(
	LPCSTR             lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData) = FindFirstFileA;

HANDLE WINAPI HookedFindFirstFileA(
	LPCSTR             lpFileName,
	LPWIN32_FIND_DATAA lpFindFileData)
{

	SPDLOG_INFO("---> FindFirstFileA({0:s}, {1})", lpFileName, "lpFindFileData");

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

	SPDLOG_INFO("<--- FindFirstFileA({0:s}, {1})", LPCSTR(newFileName.c_str()), "lpFindFileData");

	return OGFindFirstFileA(LPCSTR(newFileName.c_str()), lpFindFileData);
}
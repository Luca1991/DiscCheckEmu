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

static DWORD(WINAPI* OGGetFileAttributesA)(
	LPCSTR lpFileName) = GetFileAttributesA;

DWORD WINAPI HookedGetFileAttributesA(LPCSTR lpFileName)
{
	SPDLOG_INFO("---> GetFileAttributesA({0})", lpFileName);

	for (dce::GetFileAttributesAConfig& conf : apiConfig.getFileAttributesAConfigs)
	{
		if (string_utils::areEqualIgnoreCase(lpFileName, conf.lpFileName))
		{
			SPDLOG_INFO("<--- GetFileAttributesA({0}) | Returning 0x{1:x}", lpFileName, conf.returnValue);
			return conf.returnValue;
		}
	}

	auto it = apiConfig.fileRedirections.find(string_utils::toLowercase(lpFileName));
	if (it != apiConfig.fileRedirections.end())
	{
		lpFileName = it->second.c_str();
	}

	SPDLOG_INFO("<--- GetFileAttributesA({0})", lpFileName);

	return OGGetFileAttributesA(lpFileName);
}
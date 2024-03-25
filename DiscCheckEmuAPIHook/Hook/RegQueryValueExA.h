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
#include "..\Util\RegUtils.h"
#include "..\Util\StringUtils.h"

static LSTATUS(WINAPI* OGRegQueryValueExA)(
	HKEY    hKey,
	LPCSTR  lpValueName,
	LPDWORD lpReserved,
	LPDWORD lpType,
	LPBYTE  lpData,
	LPDWORD lpcbData) = RegQueryValueExA;

LSTATUS WINAPI HookedRegQueryValueExA(
	HKEY    hKey,
	LPCSTR  lpValueName,
	LPDWORD lpReserved,
	LPDWORD lpType,
	LPBYTE  lpData,
	LPDWORD lpcbData)
{
	SPDLOG_INFO("---> RegQueryValueExA({0}, {1:s}, {2:s}, {3:s}, {4:s}, {5:s})", "hKey", lpValueName, "lpReserved", "lpType", "lpData", "lpcbData");

	for (const dce::RegQueryValueExAConfig& conf : apiConfig.regQueryValueExAConfigs)
	{
		if (string_utils::areEqualIgnoreCase(lpValueName, conf.keyName)) {
			std::string keyPath = reg_utils::getPathFromHKEY(hKey);
			if (conf.registryPath != "" && !string_utils::areEqualIgnoreCase(keyPath, conf.registryPath))
				continue;

			if (lpType != nullptr)
				*lpType = conf.keyType;
			if (lpData != nullptr && lpcbData != nullptr)
			{
				if(*lpcbData > conf.valueSize)
					*lpcbData = conf.valueSize;

				if (conf.keyType == REG_SZ)
				{
					std::copy_n(
						std::get<std::string>(conf.value).c_str(),
						*lpcbData,
						lpData);
					SPDLOG_INFO("<--- RegQueryValueExA({0:s}, {1:s}, {2:s}, 0x{3:x}, {4:s}, 0x{5:x})",
						keyPath, lpValueName, "lpReserved", *lpType, std::get<std::string>(conf.value).c_str(), *lpcbData);
				}
				else if (conf.keyType == REG_DWORD)
				{
					auto value = std::get<uint32_t>(conf.value);
					std::copy_n(
						reinterpret_cast<LPBYTE>(&value),
						*lpcbData,
						lpData);
					SPDLOG_INFO("<--- RegQueryValueExA({0:s}, {1:s}, {2:s}, 0x{3:x}, 0x{4:x}, 0x{5:x})",
						keyPath, lpValueName, "lpReserved", *lpType, std::get<uint32_t>(conf.value), *lpcbData);
				}
				else if (conf.keyType == REG_QWORD)
				{
					auto value = std::get<uint64_t>(conf.value);
					std::copy_n(
						reinterpret_cast<LPBYTE>(&value),
						*lpcbData,
						lpData);
					SPDLOG_INFO("<--- RegQueryValueExA({0:s}, {1:s}, {2:s}, 0x{3:x}, 0x{4:x}, 0x{5:x})",
						keyPath, lpValueName, "lpReserved", *lpType, std::get<uint64_t>(conf.value), *lpcbData);
				}
			}

			return conf.returnValue;
		}
		
	}

	SPDLOG_INFO("<--- RegQueryValueExA({0}, {1:s}, {2:s}, {3:s}, {4:s}, {5:s})",
		"hKey", lpValueName, "lpReserved", "lpType", "lpData", "lpcbData");

	return OGRegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
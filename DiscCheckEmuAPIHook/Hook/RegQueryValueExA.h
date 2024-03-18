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
#ifndef NDEBUG
	std::cout << "---> RegQueryValueExA(" << hKey << ", " <<
		(lpValueName != nullptr ? lpValueName : "NULL") << ", " <<
		lpReserved << ", " <<
		lpType << ", " <<
		lpData << ", " <<
		*lpcbData << ")" << std::endl;
#endif

	for (const dce::RegQueryValueExAConfig& conf : apiConfig.regQueryValueExAConfigs)
	{
		if (string_utils::areEqualIgnoreCase(lpValueName, conf.keyName)) {
			if (conf.registryPath == "" || !string_utils::areEqualIgnoreCase(reg_utils::getPathFromHKEY(hKey), conf.registryPath))
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
				}
				else if (conf.keyType == REG_DWORD)
				{
					std::memcpy(lpData, &std::get<uint32_t>(conf.value), *lpcbData);
				}
				else if (conf.keyType == REG_QWORD)
				{
					std::memcpy(lpData, &std::get<uint64_t>(conf.value), *lpcbData);
				}
	
				
			}
#ifndef NDEBUG
			std::cout << "<--- RegQueryValueExA(" << hKey << ", " <<
				(lpValueName != nullptr ? lpValueName : "NULL") << ", " <<
				lpReserved << ", " <<
				lpType << ", " <<
				lpData << ", " <<
				*lpcbData << ")" << std::endl;
#endif

			return conf.returnValue;
		}
		
	}

#ifndef NDEBUG
	std::cout << "<--- RegQueryValueExA(" << hKey << ", " <<
		(lpValueName != nullptr ? lpValueName : "NULL") << ", " <<
		lpReserved << ", " <<
		lpType << ", " <<
		lpData << ", " <<
		lpcbData << ")" << std::endl;
#endif

	return OGRegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
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

static BOOL(WINAPI* OGGetDiskFreeSpaceA)(
	LPCSTR  lpRootPathName,
	LPDWORD lpSectorsPerCluster,
	LPDWORD lpBytesPerSector,
	LPDWORD lpNumberOfFreeClusters,
	LPDWORD lpTotalNumberOfClusters) = GetDiskFreeSpaceA;

BOOL WINAPI HookedGetDiskFreeSpaceA(
	LPCSTR  lpRootPathName,
	LPDWORD lpSectorsPerCluster,
	LPDWORD lpBytesPerSector,
	LPDWORD lpNumberOfFreeClusters,
	LPDWORD lpTotalNumberOfClusters)
{
#ifndef NDEBUG
	std::cout << "---> GetDiskFreeSpaceA(" << (lpRootPathName != nullptr ? lpRootPathName : "NULL") << ", " <<
		lpSectorsPerCluster << ", " << lpBytesPerSector << ", " << lpNumberOfFreeClusters << ", " <<
		lpTotalNumberOfClusters << ")" << std::endl;
#endif

	for (dce::GetDiskFreeSpaceAConfig& conf : apiConfig.getDiskFreeSpaceAConfigs)
	{
		if (lpRootPathName == conf.lpRootPathName)
		{
			if(lpSectorsPerCluster != nullptr)
				*lpSectorsPerCluster = conf.lpSectorsPerCluster;
			if(lpBytesPerSector != nullptr)
				*lpBytesPerSector = conf.lpBytesPerSector;
			if(lpNumberOfFreeClusters != nullptr)
				*lpNumberOfFreeClusters = conf.lpNumberOfFreeClusters;
			if(lpTotalNumberOfClusters != nullptr)
				*lpTotalNumberOfClusters = conf.lpTotalNumberOfClusters;
			return conf.returnValue;
		}
	}

#ifndef NDEBUG
	std::cout << "<--- GetDiskFreeSpaceA(" << (lpRootPathName != nullptr ? lpRootPathName : "NULL") << ", " <<
		lpSectorsPerCluster << ", " <<	lpBytesPerSector << ", " <<	lpNumberOfFreeClusters  << ", " <<
		lpTotalNumberOfClusters << ")" << std::endl;
#endif

	return OGGetDiskFreeSpaceA(lpRootPathName,
		lpSectorsPerCluster, lpBytesPerSector,
		lpNumberOfFreeClusters,	lpTotalNumberOfClusters);
	
}
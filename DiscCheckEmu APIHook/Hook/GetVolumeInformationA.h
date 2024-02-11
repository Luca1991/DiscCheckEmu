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
#include <strsafe.h>
#include "..\ApiHook.h"

static BOOL(WINAPI* OGGetVolumeInformationA)(LPCSTR lpRootPathName,
    LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
    LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength,
    LPDWORD lpFileSystemFlags, LPSTR lpFileSystemNameBuffer,
    DWORD nFileSystemNameSize) = GetVolumeInformationA;



BOOL WINAPI HookedGetVolumeInformationA(LPCSTR lpRootPathName,
    LPSTR lpVolumeNameBuffer, DWORD nVolumeNameSize,
    LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength,
    LPDWORD lpFileSystemFlags, LPSTR lpFileSystemNameBuffer,
    DWORD nFileSystemNameSize)
{
    for (DCE::GetVolumeInformationAConfig conf : apiConfig.getVolumeInformationAConfigs)
    {
        if (lstrcmp(lpRootPathName, conf.lpRootPathName.c_str()) == 0)
        {
            StringCchCopyA(
                lpVolumeNameBuffer,
                conf.nVolumeNameSize > 0 ? conf.nVolumeNameSize : nVolumeNameSize,
                conf.lpVolumeNameBuffer.c_str()
            );
            lpVolumeSerialNumber = &conf.lpVolumeSerialNumber;
            lpMaximumComponentLength = &conf.lpMaximumComponentLength;
            lpFileSystemFlags = &conf.lpFileSystemFlags;
            if (lpFileSystemNameBuffer != NULL)
            {
                StringCchCopyA(
                    lpFileSystemNameBuffer,
                    conf.nFileSystemNameSize > 0 ? conf.nFileSystemNameSize : nFileSystemNameSize,
                    conf.lpFileSystemNameBuffer.c_str()
                );
            }
            return conf.returnValue;
        }
    }

    return OGGetVolumeInformationA(lpRootPathName,
        lpVolumeNameBuffer, nVolumeNameSize,
        lpVolumeSerialNumber, lpMaximumComponentLength,
        lpFileSystemFlags, lpFileSystemNameBuffer,
        nFileSystemNameSize);
}
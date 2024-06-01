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

#include <Windows.h>
#include <detours/detours.h>
#include "Detours.h"
#include "Hook/FindFirstFileA.h"
#include "Hook/FindFirstFileW.h"
#include "Hook/CreateFileA.h"
#include "Hook/GetDiskFreeSpaceA.h"
#include "Hook/GetDriveTypeA.h"
#include "Hook/GetFileAttributesA.h"
#include "Hook/GetFileAttributesW.h"
#include "Hook/GetLogicalDrives.h"
#include "Hook/GetVolumeInformationA.h"
#include "Hook/mciSendCommand.h"
#include "Hook/RegEnumValueA.h"
#include "Hook/RegQueryValueExA.h"

namespace dce {
	void Detours::installHooks()
	{
		if (!apiConfig.fileRedirections.empty() || !apiConfig.directoryRedirections.empty())
		{
			DetourAttach(reinterpret_cast<PVOID*>(&OGFindFirstFileA), HookedFindFirstFileA);
			DetourAttach(reinterpret_cast<PVOID*>(&OGFindFirstFileW), HookedFindFirstFileW);
			DetourAttach(reinterpret_cast<PVOID*>(&OGCreateFileA), HookedCreateFileA);
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetFileAttributesW), HookedGetFileAttributesW);
		}
		if (!apiConfig.getDiskFreeSpaceAConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetDiskFreeSpaceA), HookedGetDiskFreeSpaceA);
		if (!apiConfig.getDriveAConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetDriveTypeA), HookedGetDriveTypeA);
		if (!apiConfig.getVolumeInformationAConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetVolumeInformationA), HookedGetVolumeInformationA);			
		if (!apiConfig.getFileAttributesAConfigs.empty() || !apiConfig.fileRedirections.empty() || !apiConfig.directoryRedirections.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetFileAttributesA), HookedGetFileAttributesA);
		if (!apiConfig.mciSendCommandConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGmciSendCommand), HookedmciSendCommand);
		if (!apiConfig.regEnumValueAConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGRegEnumValueA), HookedRegEnumValueA);
		if (!apiConfig.regQueryValueExAConfigs.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGRegQueryValueExA), HookedRegQueryValueExA);
		if (!apiConfig.virtualDrives.empty())
			DetourAttach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
	}

	void Detours::uninstallHooks()
	{
		if (!apiConfig.fileRedirections.empty() || !apiConfig.directoryRedirections.empty())
		{
			DetourDetach(reinterpret_cast<PVOID*>(&OGFindFirstFileA), HookedFindFirstFileA);
			DetourDetach(reinterpret_cast<PVOID*>(&OGFindFirstFileW), HookedFindFirstFileW);
			DetourDetach(reinterpret_cast<PVOID*>(&OGCreateFileA), HookedCreateFileA);
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetFileAttributesW), HookedGetFileAttributesW);
		}
		if (!apiConfig.getDiskFreeSpaceAConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetDiskFreeSpaceA), HookedGetDiskFreeSpaceA);
		if (!apiConfig.getDriveAConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetDriveTypeA), HookedGetDriveTypeA);
		if (!apiConfig.getVolumeInformationAConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetVolumeInformationA), HookedGetVolumeInformationA);			
		if (!apiConfig.getFileAttributesAConfigs.empty() || !apiConfig.fileRedirections.empty() || !apiConfig.directoryRedirections.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetFileAttributesA), HookedGetFileAttributesA);
		if (!apiConfig.mciSendCommandConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGmciSendCommand), HookedmciSendCommand);
		if (!apiConfig.regEnumValueAConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGRegEnumValueA), HookedRegEnumValueA);
		if (!apiConfig.regQueryValueExAConfigs.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGRegQueryValueExA), HookedRegQueryValueExA);
		if (!apiConfig.virtualDrives.empty())
			DetourDetach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
	}

	bool Detours::init()
	{
		if (DetourIsHelperProcess())
		{
			return true;
		}

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		installHooks();

		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}

		return true;
	}

	bool Detours::deinit()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		uninstallHooks();

		if (DetourTransactionCommit() != NO_ERROR)
		{
			return false;
		}

		return true;
	}
}
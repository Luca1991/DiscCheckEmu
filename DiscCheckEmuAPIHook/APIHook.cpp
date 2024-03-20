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
#include "APIHook.h"
#include "Config/ConfigParser.h"
#include "Hook/FindFirstFileA.h"
#include "Hook/CreateFileA.h"
#include "Hook/GetDiskFreeSpaceA.h"
#include "Hook/GetDriveTypeA.h"
#include "Hook/GetFileAttributesA.h"
#include "Hook/GetLogicalDrives.h"
#include "Hook/GetVolumeInformationA.h"
#include "Hook/mciSendCommand.h"
#include "Hook/RegQueryValueExA.h"


void installHooks()
{
    if (!apiConfig.fileRedirections.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGFindFirstFileA), HookedFindFirstFileA);
    if (!apiConfig.getDiskFreeSpaceAConfigs.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetDiskFreeSpaceA), HookedGetDiskFreeSpaceA);
    if (!apiConfig.getDriveAConfigs.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetDriveTypeA), HookedGetDriveTypeA);
    if (!apiConfig.getVolumeInformationAConfigs.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetVolumeInformationA), HookedGetVolumeInformationA);
    if(!apiConfig.fileRedirections.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGCreateFileA), HookedCreateFileA);
    if (!apiConfig.getFileAttributesAConfigs.empty() || !apiConfig.fileRedirections.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetFileAttributesA), HookedGetFileAttributesA);
    if (!apiConfig.mciSendCommandConfigs.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGmciSendCommand), HookedmciSendCommand);
    if(!apiConfig.regQueryValueExAConfigs.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGRegQueryValueExA), HookedRegQueryValueExA);
    if (!apiConfig.virtualDrives.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
}

void uninstallHooks()
{
    if (!apiConfig.fileRedirections.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGFindFirstFileA), HookedFindFirstFileA);
    if (!apiConfig.getDiskFreeSpaceAConfigs.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetDiskFreeSpaceA), HookedGetDiskFreeSpaceA);
    if (!apiConfig.getDriveAConfigs.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetDriveTypeA), HookedGetDriveTypeA);
    if (!apiConfig.getVolumeInformationAConfigs.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetVolumeInformationA), HookedGetVolumeInformationA);
    if (!apiConfig.fileRedirections.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGCreateFileA), HookedCreateFileA);
    if (!apiConfig.getFileAttributesAConfigs.empty() || !apiConfig.fileRedirections.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetFileAttributesA), HookedGetFileAttributesA);
    if (!apiConfig.mciSendCommandConfigs.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGmciSendCommand), HookedmciSendCommand);
    if (!apiConfig.regQueryValueExAConfigs.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGRegQueryValueExA), HookedRegQueryValueExA);
    if (!apiConfig.virtualDrives.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
}


BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinst, DWORD dwReason, [[maybe_unused]] LPVOID reserved)
{
    if (DetourIsHelperProcess()) {
        return true;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
#ifndef NDEBUG
        AllocConsole();
        FILE* unusedFH;
        if (freopen_s(&unusedFH, "CONOUT$", "w", stdout) != 0) {
            MessageBoxA(nullptr, "Warning: Unable to create logging console", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
        }
#endif
        try {
            dce::ConfigParser conf = dce::ConfigParser("DCEConfig.yaml");
            apiConfig = conf.parseHooks();
        }
        catch (const std::exception& e)
        {
            MessageBoxA(nullptr, e.what(), "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(1);
        }

        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        installHooks();
        
        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(nullptr, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(1);
        }
        
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
#ifndef NDEBUG
        FreeConsole();
#endif
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        uninstallHooks();

        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(nullptr, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(1);
        }
    }
    return true;
}

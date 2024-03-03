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

#include "APIHook.h"
#include <Windows.h>
#include <detours/detours.h>
#include "Config/ConfigParser.h"
#include "Hook/GetDriveTypeA.h"
#include "Hook/GetVolumeInformationA.h"
#include "Hook/CreateFileA.h"
#include "Hook/GetFileAttributesA.h"
#include "Hook/mciSendCommand.h"
#include "Hook/GetLogicalDrives.h"

void installHooks()
{
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
    if (!apiConfig.virtualDrives.empty())
        DetourAttach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
}

void uninstallHooks()
{
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
    if (!apiConfig.virtualDrives.empty())
        DetourDetach(reinterpret_cast<PVOID*>(&OGGetLogicalDrives), HookedGetLogicalDrives);
}


BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    if (DetourIsHelperProcess()) {
        return true;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        try {
            dce::ConfigParser conf = dce::ConfigParser("DCEConfig.yaml");
            apiConfig = conf.parseHooks();
        }
        catch (std::exception& e)
        {
            MessageBoxA(nullptr, e.what(), "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(-1);
        }

        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        installHooks();
        
        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(nullptr, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(-1);
        }
        
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        uninstallHooks();

        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(nullptr, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            ExitProcess(-1);
        }
    }
    return true;
}

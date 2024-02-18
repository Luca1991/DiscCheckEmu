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
#include "ConfigParser.h"
#include "Hook/GetDriveTypeA.h"
#include "Hook/GetVolumeInformationA.h"
#include "Hook/CreateFileA.h"
#include "Hook/GetFileAttributesA.h"
#include "Hook/mciSendCommand.h"
#include "Hook/GetLogicalDrives.h"

void installHooks()
{
    if(apiConfig.getDriveAConfigs.size() > 0)
        DetourAttach(&(PVOID&)OGGetDriveTypeA, HookedGetDriveTypeA);
    if(apiConfig.getVolumeInformationAConfigs.size() > 0)
        DetourAttach(&(PVOID&)OGGetVolumeInformationA, HookedGetVolumeInformationA);
    if (apiConfig.fileRedirections.size() > 0)
    {
        DetourAttach(&(PVOID&)OGCreateFileA, HookedCreateFileA);
        DetourAttach(&(PVOID&)OGGetFileAttributesA, HookedGetFileAttributesA);
    }
    if (apiConfig.mciSendCommandConfigs.size() > 0)
        DetourAttach(&(PVOID&)OGmciSendCommand, HookedmciSendCommand);
    if (apiConfig.virtualDrives.size() > 0)
        DetourAttach(&(PVOID&)OGGetLogicalDrives, HookedGetLogicalDrives);
}

void uninstallHooks()
{
    if (apiConfig.getDriveAConfigs.size() > 0)
        DetourDetach(&(PVOID&)OGGetDriveTypeA, HookedGetDriveTypeA);
    if (apiConfig.getVolumeInformationAConfigs.size() > 0)
        DetourDetach(&(PVOID&)OGGetVolumeInformationA, HookedGetVolumeInformationA);
    if (apiConfig.fileRedirections.size() > 0)
    {
        DetourDetach(&(PVOID&)OGCreateFileA, HookedCreateFileA);
        DetourDetach(&(PVOID&)OGGetFileAttributesA, HookedGetFileAttributesA);
    }
    if (apiConfig.mciSendCommandConfigs.size() > 0)
        DetourDetach(&(PVOID&)OGmciSendCommand, HookedmciSendCommand);
    if (apiConfig.virtualDrives.size() > 0)
        DetourDetach(&(PVOID&)OGGetLogicalDrives, HookedGetLogicalDrives);
}


BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        try {
            dce::ConfigParser conf = dce::ConfigParser("DCEConfig.yaml");
            apiConfig = conf.parseHooks();
        }
        catch (std::exception e)
        {
            MessageBoxA(NULL, e.what(), "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            exit(-1);
        }

        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        installHooks();
        
        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(NULL, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            return -1;
        }
        
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        uninstallHooks();
        if (DetourTransactionCommit() != NO_ERROR) {
            MessageBoxA(NULL, "Error: DetourTransactionCommit failed", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
            return -1;
        }
    }
    return TRUE;
}

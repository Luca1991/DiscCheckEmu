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
#include "APIHook.h"
#include "Config/ConfigParser.h"
#include "Hook/Engine/Detours/Detours.h"
#include "Util/MemoryUtils.h"
#include "Util/UIUtils.h"


BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE hinst, DWORD dwReason, [[maybe_unused]] LPVOID reserved)
{

    if (dwReason == DLL_PROCESS_ATTACH)
    {
#ifndef NDEBUG
        AllocConsole();
        FILE* unusedFH;
        if (freopen_s(&unusedFH, "CONOUT$", "w", stdout) != 0)
        {
            MessageBoxA(nullptr, "Warning: Unable to create logging console", "DiscCheckEmu ApiHook", MB_OK | MB_ICONERROR);
        }
#endif
        std::vector<dce::Patch> patches;
        try
        {
            dce::ConfigParser conf = dce::ConfigParser("DCEConfig.yaml");
            apiConfig = conf.parseHooks();
            patches = conf.parsePatches();
        }
        catch (const std::exception& e)
        {
            ui_utils::notifyAndExit(e.what(), "DiscCheckEmu ApiHook");
        }
        
        hookingEngine = std::make_unique<dce::Detours>();

        if(!hookingEngine->init())
        {
            ui_utils::notifyAndExit("Error: Hooking engine failed to initialize", "DiscCheckEmu ApiHook");
        }

        for (const auto& patch : patches)
        {
            SPDLOG_INFO("--- Applying patch at address: {0:x}", patch.address);
            memory_utils::applyPatch(patch.address, patch.bytes);
		}
        
        return true;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
#ifndef NDEBUG
        FreeConsole();
#endif
        if (!hookingEngine->deinit())
        {
            ui_utils::notifyAndExit("Error: Hooking engine failed to deinitialize", "DiscCheckEmu ApiHook");
        }

        return true;
    }
    return true;
}

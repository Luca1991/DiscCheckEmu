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
#include "Hack/Cheat.h"
#include "Res/CheatActivatedWAV.h"
#include "Res/CheatDeactivatedWAV.h"
#include "Util/MemoryUtils.h"

namespace dce {
	std::atomic<bool> runCheatLoop(false);

	void cheatLoop(std::vector<dce::Cheat>& cheats) {
		while(runCheatLoop.load())
		{
			for(auto& cheat : cheats)
			{
				if (GetAsyncKeyState(cheat.hotkey) & 0x1)
				{
					if (cheat.isSingleShot)
					{
						memory_utils::applyPatch(cheat.address, cheat.cheatBytes);
						PlaySound(reinterpret_cast<LPCSTR>(cheatActivatedWAV), nullptr, SND_MEMORY | SND_ASYNC);
					}
					else {
						if (!cheat.enabled)
						{
							memory_utils::applyPatch(cheat.address, cheat.cheatBytes);
							cheat.enabled = true;
							PlaySound(reinterpret_cast<LPCSTR>(cheatActivatedWAV), nullptr, SND_MEMORY | SND_ASYNC);
						}
						else {
							memory_utils::applyPatch(cheat.address, cheat.originalBytes);
							cheat.enabled = false;
							PlaySound(reinterpret_cast<LPCSTR>(cheatDeactivatedWAV), nullptr, SND_MEMORY | SND_ASYNC);
						}
					}
				}
			}
			Sleep(100);
		}
	}

}
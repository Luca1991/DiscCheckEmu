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


static MCIERROR(WINAPI* OGmciSendCommand)(
	MCIDEVICEID IDDevice,
	UINT        uMsg,
	DWORD_PTR   fdwCommand,
	DWORD_PTR   dwParam) = mciSendCommand;


MCIERROR WINAPI HookedmciSendCommand(
	MCIDEVICEID IDDevice,
	UINT        uMsg,
	DWORD_PTR   fdwCommand,
	DWORD_PTR   dwParam)
{
	SPDLOG_INFO("---> mciSendCommand(0x{0:x}, 0x{1:x}, 0x{2:x}, 0x{3:x})", IDDevice, uMsg, fdwCommand, dwParam);

	for (const dce::MciSendCommandConfig& conf : apiConfig.mciSendCommandConfigs)
	{
		if (uMsg == conf.uMsg)
		{
			if (uMsg == MCI_STATUS){
				MCI_STATUS_PARMS* par = reinterpret_cast<MCI_STATUS_PARMS*>(dwParam);
				par->dwReturn = conf.lpStatusDwReturn;
				SPDLOG_INFO("<--- mciSendCommand (MCI_STATUS) | dwReturn: 0x{0:x}", conf.lpStatusDwReturn);
			}
			SPDLOG_INFO("<--- mciSendCommand | Returning: 0x{0:x}", conf.returnValue);
			return conf.returnValue;
		}
	}

	return OGmciSendCommand(IDDevice, uMsg, fdwCommand, dwParam);
}

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
#include "ConfigParser.h"
#include "Injector.h"


int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     PSTR      lpCmdLine,
	_In_     int       nCmdShow)
{
	try
	{
		dce::ConfigParser conf = dce::ConfigParser("DCEConfig.yaml");
		dce::Injector injector = dce::Injector(conf.getTarget(), "DCEAPIHook.dll");
		injector.execute();
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "DiscCheckEmu Loader", MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}

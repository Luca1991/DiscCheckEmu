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

#include "Hook/Config/APIConfig.h"
#include "Hook/Engine/HookingEngine.h"
#ifndef NDEBUG
	#pragma warning(disable : 4996)
	#include <spdlog/spdlog.h>
	#pragma warning(default : 4996)
#else
	#define SPDLOG_INFO __noop
#endif

inline dce::APIConfig apiConfig;
inline std::unique_ptr<dce::HookingEngine> hookingEngine;
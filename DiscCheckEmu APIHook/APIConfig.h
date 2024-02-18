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

#include <vector>
#include <unordered_map>
#include "APIConfig/GetDriveAConfig.h"
#include "APIConfig/GetVolumeInformationAConfig.h"
#include "APIConfig/MciSendCommandConfig.h"

namespace dce {
	class APIConfig
	{
	public:
		std::vector<dce::GetDriveAConfig> getDriveAConfigs;
		std::vector<dce::GetVolumeInformationAConfig> getVolumeInformationAConfigs;
		std::vector<dce::MciSendCommandConfig> mciSendCommandConfigs;
		std::unordered_map<std::string, std::string> fileRedirections;
		std::vector<char> virtualDrives;
	};
}
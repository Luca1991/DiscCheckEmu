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
#include "GetDiskFreeSpaceAConfig.h"
#include "GetDriveAConfig.h"
#include "GetFileAttributesAConfig.h"
#include "GetVolumeInformationAConfig.h"
#include "MciSendCommandConfig.h"
#include "RegEnumValueAConfig.h"
#include "RegQueryValueExAConfig.h"

namespace dce {
	struct APIConfig
	{
		std::unordered_map<std::string, std::string> directoryRedirections;
		std::unordered_map<std::string, std::string> fileRedirections;
		std::vector<dce::GetDiskFreeSpaceAConfig> getDiskFreeSpaceAConfigs;
		std::vector<dce::GetDriveAConfig> getDriveAConfigs;
		std::vector<dce::GetFileAttributesAConfig> getFileAttributesAConfigs;
		std::vector<dce::GetVolumeInformationAConfig> getVolumeInformationAConfigs;
		std::vector<dce::MciSendCommandConfig> mciSendCommandConfigs;
		std::vector<dce::RegEnumValueAConfig> regEnumValueAConfigs;
		std::vector<dce::RegQueryValueExAConfig> regQueryValueExAConfigs;
		std::vector<char> virtualDrives;
	};
}
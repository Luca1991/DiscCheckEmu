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

#include "ConfigParser.h"


namespace DCE {
	ConfigParser::ConfigParser(const std::string& fileName)
	{
		config = YAML::LoadFile(fileName);
	}

	APIConfig ConfigParser::parseHooks()
	{
		YAML::Node hooks = config["hooks"];
		APIConfig apiConfig = APIConfig();

		for (const auto& hook : hooks) {
			std::string api = hook["api"].as<std::string>();

			if (api == "GetDriveTypeA")
			{
				apiConfig.getDriveAConfigs.push_back(
					GetDriveAConfig(
						hook["arg1"].as<std::string>(),
						hook["return"].as<int>()
				));

			}
			else if (api == "GetVolumeInformationA")
			{
				apiConfig.getVolumeInformationAConfigs.push_back(
					GetVolumeInformationAConfig(
						hook["arg1"].as<std::string>(),
						hook["arg2"].as<std::string>(),
						hook["arg3"].IsDefined() ? hook["arg3"].as<std::uint32_t>() : 0,
						hook["arg4"].IsDefined() ? hook["arg4"].as<std::uint32_t>() : 0,
						hook["arg5"].IsDefined() ? hook["arg5"].as<std::uint32_t>() : 0,
						hook["arg6"].IsDefined() ? hook["arg6"].as<std::uint32_t>() : 0,
						hook["arg7"].IsDefined() ? hook["arg7"].as<std::string>() : "",
						hook["arg8"].IsDefined() ? hook["arg8"].as<std::uint32_t>() : 0,
						hook["return"].as<bool>()
				));
			}
			else
				throw std::exception("Error: Unknown or unsupported API found");
		}

		return apiConfig;
	}
}
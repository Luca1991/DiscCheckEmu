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
#include "Util/StringUtils.h"


namespace DCE {
	ConfigParser::ConfigParser(const std::string& fileName)
	{
		config = YAML::LoadFile(fileName);
	}

	APIConfig ConfigParser::parseHooks()
	{
		APIConfig apiConfig = APIConfig();

		YAML::Node virtualDrives = config["virtual_drives"];

		for (std::size_t i = 0; i < virtualDrives.size(); ++i) {
			char virtualDrive = std::toupper(virtualDrives[i].as<char>());
			if (virtualDrive >= 0x41 && virtualDrive <= 0x5A)
				apiConfig.virtualDrives.push_back(virtualDrive);
			else
				throw std::exception("Error: Unable to correctly parse virtual_drives node");
		}

		YAML::Node hooks = config["hooks"];

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
			else if (api == "mciSendCommand")
			{
				MciSendCommandConfig mciSendCommandConfig = MciSendCommandConfig(
					hook["arg2"].as<std::uint32_t>(),
					hook["return"].as<std::uint32_t>()
				);

				if (hook["status_return"].IsDefined())
					mciSendCommandConfig.lpStatusDwReturn = hook["status_return"].as<std::uint32_t>();

				apiConfig.mciSendCommandConfigs.push_back(mciSendCommandConfig);
			}
			else
				throw std::exception("Error: Unknown or unsupported API found");
		}

		YAML::Node fileRedirections = config["file_redirections"];

		for (const auto& fileRedirection : fileRedirections) {
			std::string source = string_utils::toLowercase(fileRedirection["source"].as<std::string>());
			std::string destination = string_utils::toLowercase(fileRedirection["destination"].as<std::string>());
			apiConfig.fileRedirections[source] = destination;
		}

		return apiConfig;
	}
}
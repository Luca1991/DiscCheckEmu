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
#include "Util/MemoryUtils.h"


namespace dce {
	ConfigParser::ConfigParser(const std::string& fileName)
	{
		config = YAML::LoadFile(fileName);
	}

	APIConfig ConfigParser::parseHooks()
	{
		APIConfig apiConfig = APIConfig();

		YAML::Node virtualDrives = config["virtual_drives"];

		for(const auto& d : virtualDrives)
		{
			char virtualDrive = static_cast<char>(std::toupper(d.as<char>()));
			if (virtualDrive >= 'A' && virtualDrive <= 'Z')
				apiConfig.virtualDrives.push_back(virtualDrive);
			else
				throw std::exception("Error: Unable to correctly parse virtual_drives node");
		}

		YAML::Node hooks = config["hooks"];

		for (const auto& hook : hooks)
		{
			std::string api = hook["api"].as<std::string>();

			if (api == "GetDiskFreeSpaceA")
			{
				apiConfig.getDiskFreeSpaceAConfigs.push_back({
						hook["arg1"].as<std::string>(),
						hook["arg2"].as<std::uint32_t>(),
						hook["arg3"].as<std::uint32_t>(),
						hook["arg4"].as<std::uint32_t>(),
						hook["arg5"].as<std::uint32_t>(),
						hook["return"].as<bool>()
					});
			}
			else if (api == "GetDriveTypeA")
			{
				apiConfig.getDriveAConfigs.push_back(
					{
						hook["arg1"].as<std::string>(),
						hook["return"].as<int>(),
					}
				);

			}
			else if (api == "GetFileAttributesA")
			{
				apiConfig.getFileAttributesAConfigs.push_back({
						hook["arg1"].as<std::string>(),
						hook["return"].as<uint32_t>()
					});
			}
			else if (api == "GetVolumeInformationA")
			{
				apiConfig.getVolumeInformationAConfigs.push_back({
						hook["arg1"].as<std::string>(),
						hook["arg2"].as<std::string>(),
						hook["arg3"].IsDefined() ? hook["arg3"].as<std::uint32_t>() : 0,
						hook["arg4"].IsDefined() ? hook["arg4"].as<std::uint32_t>() : 0,
						hook["arg5"].IsDefined() ? hook["arg5"].as<std::uint32_t>() : 0,
						hook["arg6"].IsDefined() ? hook["arg6"].as<std::uint32_t>() : 0,
						hook["arg7"].IsDefined() ? hook["arg7"].as<std::string>() : "",
						hook["arg8"].IsDefined() ? hook["arg8"].as<std::uint32_t>() : 0,
						hook["return"].as<bool>()
					});
			}
			else if (api == "mciSendCommand")
			{
				MciSendCommandConfig mciSendCommandConfig{
					hook["arg2"].as<std::uint32_t>(),
					hook["return"].as<std::uint32_t>()
				};

				if (hook["status_return"].IsDefined())
					mciSendCommandConfig.lpStatusDwReturn = hook["status_return"].as<std::uint32_t>();

				apiConfig.mciSendCommandConfigs.push_back(mciSendCommandConfig);
			}
			else if (api == "RegEnumValueA")
			{
				uint32_t keyType = hook["key_type"].as<std::uint32_t>();
				std::variant<std::string, std::uint32_t, std::uint64_t> value;
				if (keyType == REG_SZ)
					value = hook["value"].as<std::string>();
				else if (keyType == REG_DWORD)
					value = hook["value"].as<std::uint32_t>();
				else if (keyType == REG_QWORD)
					value = hook["value"].as<std::uint64_t>();
				else
					throw std::exception("Error: Unknown or unsupported RegEnumValueA key type");

				apiConfig.regEnumValueAConfigs.push_back({
					hook["reg_path"].IsDefined() ? hook["reg_path"].as<std::string>() : "",
					hook["key_name"].IsDefined() ? hook["key_name"].as<std::string>() : nullptr,
					keyType,
					value,
					hook["value_size"].as<std::uint32_t>(),
					hook["return"].as<long>()
					});
			}
			else if (api == "RegQueryValueExA")
			{	
				uint32_t keyType = hook["key_type"].as<std::uint32_t>();
				std::variant<std::string, std::uint32_t, std::uint64_t> value;
				if (keyType == REG_SZ)
					value = hook["value"].as<std::string>();
				else if (keyType == REG_DWORD)
					value = hook["value"].as<std::uint32_t>();
				else if (keyType == REG_QWORD)
					value = hook["value"].as<std::uint64_t>();
				else 
					throw std::exception("Error: Unknown or unsupported RegQueryValueExA key type");

				apiConfig.regQueryValueExAConfigs.push_back({
					hook["reg_path"].IsDefined() ? hook["reg_path"].as<std::string>() : "",
					hook["key_name"].IsDefined() ? hook["key_name"].as<std::string>() : nullptr,
					keyType,
					value,
					hook["value_size"].as<std::uint32_t>(),
					hook["return"].as<long>()
					});
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

	std::vector<dce::Patch> ConfigParser::parsePatches()
	{
		std::vector<dce::Patch> patches;

		YAML::Node patchesNode = config["patches"];

		for (const auto& patch : patchesNode)
		{
			dce::Patch p;
			if (patch["offset"].IsDefined())
				p.address = memory_utils::getVAFromOffset(patch["offset"].as<std::size_t>());
			else if (patch["address"].IsDefined())
				p.address = reinterpret_cast<uint8_t*>(patch["address"].as<std::size_t>());
			else
				throw std::exception("Error: Patches should have an offset or an address");

			p.bytes = patch["patch"].as<std::vector<std::uint8_t>>();
			patches.push_back(p);
		}

		return patches;
	}

}
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


#include "MemoryUtils.h"
#include <Windows.h>
#include <Psapi.h>

uint8_t* memory_utils::getVAFromOffset(std::size_t offset)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	if (hModule == nullptr)
	{
		throw std::exception("GetVAFromOffset error: failed to get module handle");
	}

	MODULEINFO moduleInfo;
	if (GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)) == 0)
	{
		throw std::exception("GetModuleInformation error: failed to get module information");
	}

	IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(hModule);
	IMAGE_NT_HEADERS* ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uintptr_t>(dosHeader) + dosHeader->e_lfanew);
	std::uintptr_t imageBase = reinterpret_cast<std::uintptr_t>(moduleInfo.lpBaseOfDll);
	IMAGE_SECTION_HEADER* sectionHeader = IMAGE_FIRST_SECTION(ntHeader);

	for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i)
	{
		DWORD sectionOffset = sectionHeader[i].PointerToRawData;
		DWORD sectionSize = sectionHeader[i].SizeOfRawData;
		DWORD sectionRVA = sectionHeader[i].VirtualAddress;

		if (offset >= sectionOffset && offset < sectionOffset + sectionSize)
		{
			DWORD offsetWithinSection = offset - sectionOffset;
			return reinterpret_cast<uint8_t*>(imageBase + sectionRVA + offsetWithinSection);
		}
	}

	throw std::exception("GetVAFromOffset error: failed to get VA from offset");
}

void memory_utils::applyPatch(uint8_t* address, const std::vector<std::uint8_t>& bytes)
{
	DWORD oldProtect;
	VirtualProtect(reinterpret_cast<void*>(address), bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
	std::copy_n(bytes.data(), bytes.size(), address);
	VirtualProtect(reinterpret_cast<void*>(address), bytes.size(), oldProtect, &oldProtect);
}


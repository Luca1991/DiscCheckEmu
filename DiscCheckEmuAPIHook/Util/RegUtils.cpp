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

#include "RegUtils.h"
#include "StringUtils.h"
#include <Windows.h>
#include <vector>

struct KEY_NAME_INFORMATION {
    ULONG NameLength;
    WCHAR Name[1];
};

std::string reg_utils::getPathFromHKEY(HKEY hKey)
{
    static constexpr int KEY_NAME_INFORMATION_ID = 3;
    static constexpr auto STATUS_BUFFER_TOO_SMALL = 0xC0000023L;
    
    if (hKey == nullptr)
    {
        return {};
    }

    HMODULE dll = GetModuleHandleA("ntdll.dll");
    if (dll == nullptr) 
    {
        return {};
    }

    typedef DWORD(__stdcall* NtQueryKeyType)(
        HANDLE  KeyHandle,
        int KeyInformationClass,
        PVOID  KeyInformation,
        ULONG  Length,
        PULONG  ResultLength);

    NtQueryKeyType ntQueryKey = reinterpret_cast<NtQueryKeyType>(GetProcAddress(dll, "NtQueryKey"));
    if (ntQueryKey == nullptr) {
        return {};
    }

    DWORD size = 0;
    DWORD result = 0;
    result = ntQueryKey(hKey, KEY_NAME_INFORMATION_ID, nullptr, 0, &size);
    if (result != STATUS_BUFFER_TOO_SMALL)
    {
        return {};
    }

    size += sizeof(wchar_t);
    std::vector<wchar_t> buffer(size / sizeof(wchar_t));
    result = ntQueryKey(hKey, KEY_NAME_INFORMATION_ID, buffer.data(), size, &size);
    if (result != ERROR_SUCCESS)
    {
        return {};
    }

    auto *KNInfo = reinterpret_cast<KEY_NAME_INFORMATION*>(buffer.data());
    std::wstring keyPathW(reinterpret_cast<const wchar_t*>(&KNInfo->Name), KNInfo->NameLength / sizeof(wchar_t));
    return string_utils::widestringToString(keyPathW);
}


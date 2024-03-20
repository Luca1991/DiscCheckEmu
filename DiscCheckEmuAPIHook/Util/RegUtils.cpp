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


std::string reg_utils::getPathFromHKEY(HKEY hKey)
{
    std::wstring keyPathW;
    if (hKey != nullptr)
    {
        HMODULE dll = LoadLibraryA("ntdll.dll");
        if (dll != nullptr) {
            typedef DWORD(__stdcall* NtQueryKeyType)(
                HANDLE  KeyHandle,
                int KeyInformationClass,
                PVOID  KeyInformation,
                ULONG  Length,
                PULONG  ResultLength);

            NtQueryKeyType func = reinterpret_cast<NtQueryKeyType>(GetProcAddress(dll, "NtQueryKey"));

            if (func != nullptr) {
                wchar_t buffer[MAX_PATH] = { '/0' };
                DWORD size = MAX_PATH;
                DWORD result = 0;
                result = func(hKey, 3, buffer, size, &size);
                keyPathW = std::wstring(buffer + 2);
            }

            FreeLibrary(dll);
        }
    }

    int keyPathSize = WideCharToMultiByte(CP_UTF8, 0, keyPathW.data(), keyPathW.size(), nullptr, 0, nullptr, nullptr);
    std::string keyPath(keyPathSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, keyPathW.data(), keyPathW.size(), keyPath.data(), keyPathSize, nullptr, nullptr);
    return keyPath;
}


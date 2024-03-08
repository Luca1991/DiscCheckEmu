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

#pragma warning( disable : 6335 )

#include "Injector.h"
#include <psapi.h> 
#include <array>

namespace dce {

	Injector::Injector(const std::string& targetName, const std::string& dllName)
	{
		this->targetName = targetName;
		this->dllName = dllName;
	}

	PROCESS_INFORMATION Injector::createTargetProcess()
	{
		STARTUPINFO startupInfo{};
		startupInfo.cb = sizeof(startupInfo);
		PROCESS_INFORMATION processInformation{};

		if (!CreateProcessA(
			targetName.c_str(),
			nullptr,
			nullptr,
			nullptr,
			false,
			CREATE_SUSPENDED,
			nullptr,
			nullptr,
			&startupInfo,
			&processInformation))
		{
			throw std::exception("Error: Can't create the target proccess");
		}

		return processInformation;
	}

	LPVOID Injector::writeDllPathToProcess(PROCESS_INFORMATION processInformation)
	{
		LPVOID startAddr = VirtualAllocEx(
			processInformation.hProcess,
			nullptr,
			dllName.size() + 1,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE
		);
		if (startAddr == nullptr)
		{
			TerminateProcess(processInformation.hProcess, 0);
			throw std::exception("Error: Can't allocate space to the target proccess");
		}

		if (!WriteProcessMemory(processInformation.hProcess,
			startAddr,
			dllName.c_str(),
			dllName.size(),
			nullptr))
		{
			TerminateProcess(processInformation.hProcess, 0);
			throw std::exception("Error: Can't write to the target proccess");
		}

		return startAddr;
	}

	void Injector::injectAndResume(LPVOID startAddr,
		PROCESS_INFORMATION processInformation)
	{
		HMODULE k32handle = GetModuleHandleA("Kernel32.dll");
		if (k32handle == nullptr)
		{
			TerminateProcess(processInformation.hProcess, 0);
			throw std::exception("Error: Can't get Kernel32 Module Handle");
		}

		FARPROC loadLibraryAAddr = GetProcAddress(k32handle, "LoadLibraryA");
		if (loadLibraryAAddr == nullptr)
		{
			TerminateProcess(processInformation.hProcess, 0);
			throw std::exception("Error: Can't get LoadLibraryA address");
		}

		HANDLE hRemoteThread = CreateRemoteThread(
			processInformation.hProcess,
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAAddr),
			startAddr,
			0,
			nullptr);
		if(!hRemoteThread)
		{
			TerminateProcess(processInformation.hProcess, 0);
			throw std::exception("Error: Can't create remote thread");
		}

		CloseHandle(hRemoteThread);

		Sleep(3 * 1000);

		ResumeThread(processInformation.hThread);
	}

	void Injector::checkInjection(PROCESS_INFORMATION processInformation)
	{
		Sleep(3 * 1000);
		std::array<HMODULE, 1024> hModules;
		DWORD cbNeeded;
		if(EnumProcessModules(processInformation.hProcess, hModules.data(),
			sizeof(hModules), &cbNeeded))
		{
			int moduleCount = cbNeeded / sizeof(HMODULE);

			bool found = false;
			for(int i = 0; i < moduleCount && !found; ++i)
			{
				std::string szModName(MAX_PATH, 0);
				if (GetModuleFileNameEx(processInformation.hProcess,
					hModules[i], szModName.data(), (DWORD)szModName.size()))
				{
					szModName.shrink_to_fit();
					if (szModName.find(dllName) != std::string::npos)
					{
						found = true;
					}
				}
			}
			if (!found)
			{
				throw std::exception("Warning: Failed to find injected dll in target process");
			}
		}
		else
		{
			throw std::exception("Warning: Failed to check for sucesfull process injection");
		}
	}

	void Injector::execute()
	{
		PROCESS_INFORMATION processInformation = createTargetProcess();
		LPVOID startAddr = writeDllPathToProcess(processInformation);
		injectAndResume(startAddr, processInformation);
		checkInjection(processInformation);
	}
}
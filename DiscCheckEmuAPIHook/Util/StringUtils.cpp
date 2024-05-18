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

#include "StringUtils.h"
#include <algorithm>
#include <string>
#include <Windows.h>

bool string_utils::areEqualIgnoreCase(const std::string& str1, const std::string& str2)
{
	if (str1.size() != str2.size())
		return false;

	return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b)
		{
			return std::tolower(a) == std::tolower(b);
		});
}

bool string_utils::startsWithIgnoreCase(const std::string& str, const std::string& prefix)
{
	if (str.size() < prefix.size())
		return false;

	return std::equal(prefix.begin(), prefix.end(), str.begin(), [](char a, char b)
		{
			return std::tolower(a) == std::tolower(b);
		});
}

std::string string_utils::toLowercase(const std::string& str)
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		});
	return result;
}

std::string string_utils::widestringToString(const std::wstring& wideString)
{
	if (wideString.empty()) return {};

	auto size_needed = WideCharToMultiByte(CP_UTF8, 0, wideString.data(), static_cast<int>(wideString.size()), nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0)
		throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideString.data(), static_cast<int>(wideString.size()), result.data(), size_needed, nullptr, nullptr);
	return result;
}
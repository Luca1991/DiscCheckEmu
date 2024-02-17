#include "StringUtils.h"
#include <algorithm>

bool string_utils::areEqualIgnoreCase(const std::string& str1, const std::string& str2)
{
	if (str1.size() != str2.size())
		return false;

	return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b)
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
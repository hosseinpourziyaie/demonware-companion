/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : Common Utilities
** - Description : set of usefull functions gathered all together
** - Author      : Hosseinpourziyaie
**
**
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#include "stdinc.h"
#include <direct.h>

bool create_directory_if_not_exists(std::string directory)
{
	struct stat info;

	if (stat(directory.c_str(), &info) == -1) {
		if (_mkdir(directory.c_str()) == -1) return false;
	}

	return true;
}

std::string to_lower(std::string str)
{
	for (char& ch : str)
	{
		ch = tolower(ch);
	}

	return str;
}
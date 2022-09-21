#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "procHex.h"
#include "utilities.h"


#include "WorldWarII.h"
#include "MWRemastered.h"
#include "BlackOps3.h"
#include "InfiniteWarfare.h"


extern std::map<std::string, void(*)(ProcHex)> SupportedTitlesList;

enum procces_fetch_error
{
	COULDNT_FIND_PROCESS,
	COULDNT_OPEN_HANDLE,
	COULDNT_TARGET_MODULE
};
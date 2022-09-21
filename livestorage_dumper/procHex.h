/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : procHex Library Header
** - Description : Lightweight Memory library made by myself
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-09-15     | Ended on : 2022-09-20
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#pragma once
#define READSTRING_SIZE 128

class ProcHex
{
public:
	DWORD processID = 0;
	DWORD64 moduleBase = 0;
	DWORD64 moduleSize = 0;

	HANDLE processHandle = NULL;


	ProcHex();
	~ProcHex();

	ProcHex(const std::string& procName, const std::string& moduleName); // ALL IN ONE

	bool FindProcess(const std::string& procName);
	bool TargetModule(const std::string& moduleName);
	bool OpenHandle();

	bool ReadMemory(uint64_t startAddr, void* buffer, size_t length);
	bool DumpMemory(uint64_t startAddr, size_t length, std::string fileName);
	int32_t ReadInt32(uint64_t Addr);
	std::string ReadString(uint64_t Addr);

	uint64_t FollowJump(uint64_t Addr);
	uint64_t FollowLea(uint64_t Addr);
	uint64_t ElevateByBase(uint64_t Addr);

	static std::string getLastErrorSTR();

	uint64_t FindPattern(uint64_t searchStartAddr, size_t searchLength, const unsigned char* pattern, const char* mask);

private:

	bool FindPatterninDump(const unsigned char* dump, const size_t length, const unsigned char* pattern, const char* mask, uint64_t& outOffset);

};

/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : procHex Library Source
** - Description : Lightweight Memory library made by myself
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-09-15     | Ended on : 2022-09-20
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#include "stdinc.h"
#include "procHex.h"

#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>
#include <fstream>

ProcHex::ProcHex() {}

ProcHex::~ProcHex()
{
	if (this->processHandle) {
		//CloseHandle(this->processHandle); // weird exception but why?
	}
}
ProcHex::ProcHex(const std::string& procName, const std::string& moduleName)
{
	this->FindProcess(procName);
	this->OpenHandle();
	this->TargetModule(moduleName);

	// All in one constructor but its not recommended to initialize this way
}

bool ProcHex::FindProcess(const std::string& procName)
{
	bool result = false; std::wstring procName_widestr = std::wstring(procName.begin(), procName.end());
	std::transform(procName_widestr.begin(), procName_widestr.end(), procName_widestr.begin(), ::tolower);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &procEntry))
		{
			do
			{
				if (!wcscmp(_wcslwr(procEntry.szExeFile), procName_widestr.c_str()))
				{
					result = true; this->processID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &procEntry));
		}

		CloseHandle(hSnapshot);
	}
	return result;
}

bool ProcHex::TargetModule(const std::string& moduleName)
{
	bool result = false; std::wstring moduleName_widestr = std::wstring(moduleName.begin(), moduleName.end());
	std::transform(moduleName_widestr.begin(), moduleName_widestr.end(), moduleName_widestr.begin(), ::tolower);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &moduleEntry))
		{
			do
			{
				if (!wcscmp(_wcslwr(moduleEntry.szModule), moduleName_widestr.c_str()))
				{
					this->moduleBase = (DWORD64)moduleEntry.modBaseAddr;
					this->moduleSize = (DWORD64)moduleEntry.modBaseSize;

					result = true; break;
				}
			} while (Module32Next(hSnapshot, &moduleEntry));
		}

		CloseHandle(hSnapshot);
	}
	return result;
}

bool ProcHex::OpenHandle()
{
	this->processHandle = OpenProcess(PROCESS_VM_READ, FALSE, this->processID);

	if (this->processHandle == INVALID_HANDLE_VALUE) return false; // Error on opening handle to process

	return true;
}

bool ProcHex::ReadMemory(uint64_t startAddr, void* buffer, size_t length)
{
	if (this->processHandle == INVALID_HANDLE_VALUE) return false;

	bool status = ReadProcessMemory(this->processHandle, (void*)startAddr, buffer, length, NULL);

	return status;
}

bool ProcHex::DumpMemory(uint64_t startAddr, size_t length, std::string fileName)
{
	if (this->processHandle == INVALID_HANDLE_VALUE) return false;

	auto buffer = new char[length];
	if (ReadProcessMemory(this->processHandle, (void*)startAddr, buffer, length, nullptr) != 0)
	{
		const auto pos = fileName.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			if (create_directory_if_not_exists(fileName.substr(0, pos).c_str()) == false) goto FAILURE;
		}

		std::ofstream stream(fileName, std::ios::binary | std::ios::out);
		if (stream.is_open())
		{
			stream.write(buffer, length);
			stream.close();
			delete[] buffer;
			return true;
		}
	}

FAILURE:
	delete[] buffer;
	return false;
}

std::string ProcHex::ReadString(uint64_t Addr)
{
	char* szBuffer = new char[READSTRING_SIZE]();
	bool succeed = ReadMemory(Addr, szBuffer, READSTRING_SIZE);

	if (!succeed)
	{
		delete[] szBuffer; return "";
	}

#ifdef SAFE_SOLUTIONS
	std::string result;

	for (size_t i = 0; i < sizeof(szBuffer); i++)
	{
		if (szBuffer[i] == 0x00) break;
		result.push_back(szBuffer[i]);
	}
#else
	std::string result(szBuffer);     // I dont really trust this :/
#endif

	delete[] szBuffer; return result; // dispose allocated buffer and return
}

int32_t ProcHex::ReadInt32(uint64_t Addr)
{
	int result = 0;
	ReadMemory(Addr, &result, sizeof(result));
	return result;
}

uint64_t ProcHex::FollowLea(uint64_t Addr)
{
	return Addr + ReadInt32(Addr + 3) + 7;
}

uint64_t ProcHex::FollowJump(uint64_t Addr)
{
	return Addr + ReadInt32(Addr + 1) + 5;
}

uint64_t ProcHex::ElevateByBase(uint64_t Addr)
{
	return this->moduleBase + Addr;
}

uint64_t ProcHex::LowerByBase(uint64_t Addr)
{
	return Addr - this->moduleBase;
}

uint64_t ProcHex::FindPattern(uint64_t searchStartAddr, size_t searchLength, const unsigned char* pattern, const char* mask)
{
	if (this->processHandle == INVALID_HANDLE_VALUE) return 0; uint64_t result = 0;

	auto buffer = new unsigned char[searchLength];
	if (ReadProcessMemory(this->processHandle, (void*)searchStartAddr, buffer, searchLength, nullptr) != 0)
	{
		uint64_t offset;
		if (ProcHex::FindPatterninDump(buffer, searchLength, pattern, mask, offset))
		{
			result = searchStartAddr + offset;
		}
	}
	delete[] buffer;

	return result;
}

bool ProcHex::FindPatterninDump(const unsigned char* dump, const size_t length, const unsigned char* pattern, const char* mask, uint64_t& outOffset)
{
	const size_t maskLength = std::strlen(mask) - 1;
	size_t patternPos = 0;
	size_t tmpOffset = -1;
	for (size_t offset = 0; offset < length - 1; ++offset)
	{
		if (dump[offset] == pattern[patternPos] || mask[patternPos] == '?')
		{
			if (mask[patternPos + 1] == '\0')
			{
				outOffset = (offset - maskLength);
				return true;
			}
			if (tmpOffset == static_cast<size_t>(-1))
			{
				tmpOffset = offset;
			}
			patternPos++;
		}
		else
		{
			if (tmpOffset > static_cast<size_t>(-1))
			{
				offset = tmpOffset;
				tmpOffset = static_cast<size_t>(-1);
			}
			patternPos = 0;
		}
	}

	return false;
}


/* TODOLIST        : 1- Expand capability of troubleshooting with logging errors
                     2- Implement a better FindPattern function 
					 3- Write detailed user-manual documentation
					                                                          */


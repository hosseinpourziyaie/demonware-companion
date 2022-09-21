/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : BlackOps3 Handler Source
** - Description : Handler for dumping mwr livestorage files
** - Author      : Hosseinpourziyaie
**
** - Note        : define DEBUG to print out extended debug information
**
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#include "stdinc.h"

inline std::string BlackOps3::getFileOpResultSTR(dwFileOpResult num)
{
	switch (num)
	{
	case DW_FILEOP_SUCCESS: return "DW_FILEOP_SUCCESS";
	case DW_FILEOP_NOT_FOUND: return "DW_FILEOP_NOT_FOUND";
	case DW_FILEOP_FAILURE: return "DW_FILEOP_FAILURE";
	default: return "DW_FILEOP_UNKNOWN";
	}
}

std::map<std::string, uintptr_t> BlackOps3::dwFileOperationInfoList;

void BlackOps3::StartUpExtraction(ProcHex process)
{
	//printf("------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < 4; i++)
	{
		uint64_t s_onlinePubDataAddr = process.ElevateByBase(0x3410C40) + (i * sizeof(onlinePubData));
		onlinePubData* s_onlinePubData = new onlinePubData{};

		if (process.ReadMemory(s_onlinePubDataAddr, s_onlinePubData, sizeof(onlinePubData)))
		{
			std::string onlinePubDataName = process.ReadString(reinterpret_cast<uint64_t>(s_onlinePubData->fileNameType));

			printf("s_onlinePubData[%i]: %s[0x%llX]--> %p\n", i, onlinePubDataName.c_str(), s_onlinePubDataAddr, s_onlinePubData->fileOps);

			dwFileOperationInfoList.insert({ onlinePubDataName, process.LowerByBase(reinterpret_cast<uint64_t>(s_onlinePubData->fileOps)) });
		}
		delete s_onlinePubData;
	}
	printf("------------------------------------------------------------------------------------------------\n");

	dwFileOperationInfoList.insert({ "s_dbKeysInfo", 0x1189BEB0 });
	dwFileOperationInfoList.insert({ "s_ffotdInfo", 0x1189C120 });
	dwFileOperationInfoList.insert({ "s_qosInfo", 0x11412DF0 });

	for (auto const& pair : dwFileOperationInfoList) {
		dwFileOperationInfo* fileOperationInfo = new dwFileOperationInfo{};
		if (process.ReadMemory(process.ElevateByBase(pair.second), fileOperationInfo, sizeof(dwFileOperationInfo)))
		{
			std::string filename_str = process.ReadString(reinterpret_cast<uint64_t>(fileOperationInfo->fileTask.m_filename));

			if (fileOperationInfo->fileTask.m_error == 1000)
			{
				std::cout << "[INFO] [" << pair.first << "] " << filename_str << " is not provided by demonware server (error code " << fileOperationInfo->fileTask.m_error << ")" << std::endl;
			}
			else
			{
				bool succsseed = process.DumpMemory(reinterpret_cast<uint64_t>(fileOperationInfo->fileTask.m_fileData.m_fileData), fileOperationInfo->fileTask.m_fileData.m_fileSize, "t7_dwfiles\\" + filename_str);
				if(succsseed) std::cout << "[INFO] [" << pair.first << "] " << filename_str << " with total size of " << fileOperationInfo->fileTask.m_fileSize << " bytes dumped successfully" << std::endl;
				else std::cout << "[ERROR] [" << pair.first << "] failed to dump " << filename_str << "(size: " << fileOperationInfo->fileTask.m_fileSize << ")" << std::endl;
			}

#ifdef DEBUG
			printf("%s: 0x%llX [%s]\n", pair.first.c_str(), process.ElevateByBase(pair.second), getFileOpResultSTR(fileOperationInfo->result).c_str());

			printf("%s->fileTask.m_file: %s(%u)\n", pair.first.c_str(), process.ReadString(reinterpret_cast<uint64_t>(fileOperationInfo->fileTask.m_filename)).data(), fileOperationInfo->fileTask.m_fileSize);
			printf("%s->fileTask.m_fileData: %p(%u)\n", pair.first.c_str(), fileOperationInfo->fileTask.m_fileData.m_fileData, fileOperationInfo->fileTask.m_fileData.m_fileSize);
			printf("%s->fileTask.m_buffer: %p(%u)\n", pair.first.c_str(), fileOperationInfo->fileTask.m_buffer, fileOperationInfo->fileTask.m_bufferSize);
			printf("%s->fileBuffer: %p(%u)\n", pair.first.c_str(), fileOperationInfo->fileBuffer, fileOperationInfo->bufferSize);
			if (fileOperationInfo->isCompressedFile) printf("%s->decompressedSize: %u\n", pair.first.c_str(), fileOperationInfo->decompressedSize);
			if (fileOperationInfo->fileTask.m_error) printf("%s->fileTask.m_error: %u\n", pair.first.c_str(), fileOperationInfo->fileTask.m_error);

			//printf("------------------------------------------------------------------------------------------------\n");
#endif
		}
		else
		{
			std::cout << "[ERROR] failed to read dwFileOperationInfo " << pair.first << "(0x" << std::hex << std::uppercase << pair.second << std::dec << ")" << std::endl;
		}
		delete fileOperationInfo; std::cout << std::endl;
		//std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}
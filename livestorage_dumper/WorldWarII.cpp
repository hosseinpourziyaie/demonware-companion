/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : WorldWarII Handler Source
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

std::map<std::string, uintptr_t> WorldWarII::dwFileTaskList;

void WorldWarII::StartUpExtraction(ProcHex process)
{
	dwFileTaskList.insert({ "g_fetchPlaylistsFileTask", 0x1D7EE80 });
	dwFileTaskList.insert({ "g_fetchFFotDFileTask", 0xACE2270 });
	dwFileTaskList.insert({ "g_fetchMotdFileTask", 0xADB3D60 });
	dwFileTaskList.insert({ "g_downloadPublisherSurveys", 0x70BAD00 });

	dwFileTaskList.insert({ "g_fetchdwTaskConfig", 0x7096850 });
	dwFileTaskList.insert({ "g_readMetPlayersList", 0x1D27320 });


	for (auto const& pair : dwFileTaskList) {
		dwFileTask* fileTask = new dwFileTask{};
		if (process.ReadMemory(process.ElevateByBase(pair.second), fileTask, sizeof(dwFileTask)))
		{
			std::string filename_str = process.ReadString(reinterpret_cast<uint64_t>(fileTask->m_filename));

			if (fileTask->m_error == 1000)
			{
				std::cout << "[INFO] [" << pair.first << "] " << filename_str << " is not provided by demonware server (error code " << fileTask->m_error << ")" << std::endl;
			}
			else
			{
				bool succsseed =process.DumpMemory(reinterpret_cast<uint64_t>(fileTask->bdFileData), fileTask->bdFileSize, "s2_dwfiles\\" + filename_str);
				if (succsseed) std::cout << "[INFO] [" << pair.first << "] " << filename_str << " with total size of " << fileTask->m_fileSize << " bytes dumped successfully" << std::endl;
				else std::cout << "[ERROR] [" << pair.first << "] failed to dump " << filename_str << "(size: " << fileTask->m_fileSize << ")" << std::endl;
			}

#ifdef DEBUG
			if (fileTask->m_error) printf("** %i ** ", fileTask->m_error);
			printf("%s: %llX\n\n", pair.first.c_str(), process.ElevateByBase(pair.second));

			printf("%s->m_file: %s(%i)\n", pair.first.c_str(), process.ReadString(reinterpret_cast<uint64_t>(fileTask->m_filename)).data(), fileTask->m_fileSize);
			printf("%s->m_dataResult: %p(%i)\n", pair.first.c_str(), fileTask->bdFileData, fileTask->bdFileSize);
			printf("%s->m_buffer: %p(%i)\n", pair.first.c_str(), fileTask->m_buffer, fileTask->m_bufferSize);

			//printf("------------------------------------------------------------------------------------------------\n");
#endif
		}
		delete fileTask; std::cout << std::endl;
		//std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}


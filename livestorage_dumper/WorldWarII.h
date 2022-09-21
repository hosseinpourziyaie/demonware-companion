#pragma once

class WorldWarII
{
public:
	static void StartUpExtraction(ProcHex process);

private:
	static std::map<std::string, uintptr_t> dwFileTaskList;


	struct dwFileTask
	{
		char padding_unk1[8];            // 00
		void* m_filename;                // 0x8
		char padding_unk2[0x48];         // 0x10
		unsigned int m_fileSize;         // 0x58
		char padding_unk3[0xAFC];        // 0x5C
		void* bdFileData;                // 0xB58 (bdFileData->m_fileData)
		unsigned int bdFileSize;         // 0xB60 (bdFileData->m_fileSize)
		unsigned int m_error;            // 0xB64
		char padding_unk4[0x1A368];      // 0xB68
		unsigned int m_bufferSize;       // 0x1AED0
		char padding_unk5[0x24];         // 0x1AED4
		void* m_buffer;                  // 0x1AEF8
	}; // size = unknown

	static_assert(offsetof(dwFileTask, m_bufferSize) == (0x1AED0), "something wrong with dwFileTask");
	static_assert(offsetof(dwFileTask, m_buffer) == (0x1AEF8), "something wrong with dwFileTask");
};
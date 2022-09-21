#pragma once

class ModernWarfareRemastered
{
public:
	static void StartUpExtraction(ProcHex process);

private:
	static std::map<std::string, uintptr_t> dwFileTaskList;

	struct bdFileInfo
	{
		char padding_unk1[16];    // 00
		uint64_t m_fileID;        // 16
		uint32_t m_createTime;    // 24
		uint32_t m_modifedTime;   // 28
		uint32_t m_visibility;    // 32
		uint32_t unknown;         // 36
		uint64_t m_ownerID;       // 40
		char m_fileName[128];     // 48
		uint32_t m_fileSize;      // 176
		char padding_unk2[12];    // 180
	}; // size = 192

	struct bdFileData
	{
		char __pad0[16];          // 00
		void* m_fileData;         // 16 (21952 in dwFileTask)
		unsigned int m_fileSize;  // 24 (21960 in dwFileTask)
		char __pad1[4];           // 28
	}; // size = 32

	struct dwFileTask
	{
		char __pad0[8];                    // 00
		void* m_filename;                  // 08
		char __pad1[56];                   // 16
		unsigned int m_error;              // 72
		unsigned int m_fileSize;           // 76
		char __pad3[19872];                // 80
		bdFileInfo m_infoPublisherResult;  // 19952
		char __pad4[1792];                 // 20144
		bdFileData m_dataResult;           // 21936
		char __pad5[2016];                 // 21968
		unsigned int m_bufferSize;         // 23984
		char __pad6[28];                   // 23988
		void* m_buffer;                    // 24016
	}; // size = unknown

	static_assert(sizeof(bdFileInfo) == 192, "something wrong with bdFileInfo");
	static_assert(sizeof(bdFileData) == 32, "something wrong with bdFileData");
	static_assert(offsetof(dwFileTask, m_bufferSize) == (23984), "something wrong with dwFileTask");
	static_assert(offsetof(dwFileTask, m_buffer) == (24016), "something wrong with dwFileTask");
};
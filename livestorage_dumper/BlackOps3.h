#pragma once

class BlackOps3
{
public:
	static void StartUpExtraction(ProcHex process);

private:
	static std::map<std::string, uintptr_t> dwFileOperationInfoList;

	struct bdFileData
	{
		void* m_fileData;  // 24 from start of dwFileTask
		unsigned int m_fileSize;   // 32 from start of dwFileTask
		char padding_dsa[4]; // Data structure alignment
	};

	struct dwFileTask
	{
		const char* m_filename;   // 00
		char padding_unk1[16];    // 08
		bdFileData m_fileData;    // 24
		char padding_unk2[16];    // 40
		uint32_t m_error;         // 56
		uint32_t unknown;         // 60
		uint64_t m_fileID;        // 64
		uint32_t m_fileSize;      // 72
		uint32_t m_bufferSize;    // 76
		char padding_unk3[216];   // 80
		void* m_buffer;           // 296
		bool m_optional;          // 304
		char padding_dsa[7]; // 305-->312
	};

	struct dwFileOperationInfo
	{
		dwFileTask fileTask;      // 000
		char padding_unk1[16];    // 312
		bool fetchCompleted;      // 328
		bool writeCompleted;      // 329
		bool isUserFile;          // 330
		bool isCompressedFile;    // 331
		char padding_unk2[4];     // 332
		void* fileBuffer;         // 336
		uint32_t bufferSize;      // 344
	};

	struct onlinePubData
	{
		const char* fileNameType;  // 000
		char fileName[128];        // 008
		void* buffer;              // 136
		uint32_t bufferSize;       // 144
		bool isFileCompressed;     // 148
		char padding_dsa1[3];      // Data structure alignment
		void* taskDef;             // 152
		void* fileOps;             // 160
		bool dataFetchCompleted;   // 168
		bool multipleLanguages;    // 169
		char padding_dsa2[6];      // Data structure alignment
		void* successCallback;     // 176
		void* failureCallback;     // 184
	};

	static_assert(sizeof(onlinePubData) == 192, "something wrong with onlinePubData");
	static_assert(sizeof(dwFileTask) == 312, "something wrong with dwFileTask");
	static_assert(offsetof(dwFileTask, m_bufferSize) == (76), "something wrong with dwFileTask");
	static_assert(offsetof(dwFileTask, m_buffer) == (296), "something wrong with dwFileTask");
};
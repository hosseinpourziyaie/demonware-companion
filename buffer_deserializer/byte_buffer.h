#pragma once
#include <vector>
#include <string>


struct bdByteBufferReader
{
    std::vector<unsigned char> buffer;
    unsigned int current_byte;
};

enum bdByteBufferDataType
{
    BD_BB_NO_TYPE = 0,
    BD_BB_BOOL_TYPE = 1,
    BD_BB_SIGNED_CHAR8_TYPE = 2,
    BD_BB_UNSIGNED_CHAR8_TYPE = 3,
    BD_BB_SIGNED_INTEGER16_TYPE = 5,
    BD_BB_UNSIGNED_INTEGER16_TYPE = 6,
    BD_BB_SIGNED_INTEGER32_TYPE = 7,
    BD_BB_UNSIGNED_INTEGER32_TYPE = 8,
    BD_BB_SIGNED_INTEGER64_TYPE = 9,
    BD_BB_UNSIGNED_INTEGER64_TYPE = 10,
    BD_BB_SIGNED_CHAR8_STRING_TYPE = 16,
    BD_BB_BLOB_TYPE = 19
};

extern const char* bdByteBufferDataTypeNames[];


bool read(bdByteBufferReader* input, unsigned int bytes, void* output);
bool read_data_type(bdByteBufferReader* input, bdByteBufferDataType expected);
bool read_int16(bdByteBufferReader* input, int16_t* output);
bool read_uint16(bdByteBufferReader* input, uint16_t* output);
bool read_int32(bdByteBufferReader* input, int32_t* output);
bool read_uint32(bdByteBufferReader* input, uint32_t* output);
bool read_int64(bdByteBufferReader* input, int64_t* output);
bool read_uint64(bdByteBufferReader* input, uint64_t* output);

bool read_string(bdByteBufferReader* input, std::string* output);
bool read_string(bdByteBufferReader* input, char* output, int maxlen);
bool read_blob(bdByteBufferReader* input, std::vector<unsigned char>* output, int* length = 0);

bool reader_skip(bdByteBufferReader* input, unsigned int bytes);
bool reader_skip(bdByteBufferReader* input, bdByteBufferDataType type);

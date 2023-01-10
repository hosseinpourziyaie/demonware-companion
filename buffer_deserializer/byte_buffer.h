#pragma once
#include <vector>
#include <string>

enum bdByteBufferDataType
{
    BD_BB_NO_TYPE = 0,
    BD_BB_BOOL_TYPE = 1,
    BD_BB_SIGNED_CHAR8_TYPE = 2,
    BD_BB_UNSIGNED_CHAR8_TYPE = 3,
    BD_BB_WCHAR16_TYPE = 4,
    BD_BB_SIGNED_INTEGER16_TYPE = 5,
    BD_BB_UNSIGNED_INTEGER16_TYPE = 6,
    BD_BB_SIGNED_INTEGER32_TYPE = 7,
    BD_BB_UNSIGNED_INTEGER32_TYPE = 8,
    BD_BB_SIGNED_INTEGER64_TYPE = 9,
    BD_BB_UNSIGNED_INTEGER64_TYPE = 10,
    BD_BB_SIGNED_CHAR8_STRING_TYPE = 16,
    BD_BB_BLOB_TYPE = 19,
    BD_BB_STRUCTURED_DATA_TYPE = 23,
    BD_BB_MAX_TYPE = 32
};

extern const char* bdByteBufferDataTypeNames[];

class bdByteBufferReader
{
public:
    std::vector<unsigned char> buffer;
    unsigned int current_byte = 0;
    bool type_checked = true;

    bdByteBufferReader();
    ~bdByteBufferReader();

    bdByteBufferReader(std::vector<unsigned char> input);

    bool read(unsigned int bytes, void* output);
    bool read_data_type(bdByteBufferDataType expected);
    bool read_int16(int16_t* output);
    bool read_uint16(uint16_t* output);
    bool read_int32(int32_t* output);
    bool read_uint32(uint32_t* output);
    bool read_int64(int64_t* output);
    bool read_uint64(uint64_t* output);

    bool read_string(std::string* output);
    bool read_string(char* output, int maxlen);
    bool read_blob(std::vector<unsigned char>* output, int* length = 0);
    bool read_structed_data(std::vector<unsigned char>* output, int* length = 0);

    bool reader_skip(unsigned int bytes);
    bool reader_skip(bdByteBufferDataType type, bool type_included = true);
};
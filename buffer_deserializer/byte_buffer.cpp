/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : byteBuffer Helper Functions
** - Description : collection of utilities for reading out byte buffer 
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-08-25     | Ended on : 2022-08-28
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/

#include "byte_buffer.h"

const char* bdByteBufferDataTypeNames[] =
{
    "none", "bool", "signed char", "unsigned char", "wchar_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t", "", "", "float32", "float64", "", "string", "", "", "binary"
};

bool read(bdByteBufferReader* input, unsigned int bytes, void* output)
{
    if (bytes + input->current_byte > input->buffer.size()) return false;

    memcpy(output, input->buffer.data() + input->current_byte, bytes);
    input->current_byte += bytes;

    return true;
}

bool read_data_type(bdByteBufferReader* input, bdByteBufferDataType expected)
{
    if (input->current_byte + 1 > input->buffer.size()) return false;

    if (input->buffer.at(input->current_byte) != expected) return false;

    input->current_byte++;  return true;
}

bool read_int16(bdByteBufferReader* input, int16_t* output)
{
    if (!read_data_type(input, BD_BB_SIGNED_INTEGER16_TYPE)) return false;
    return read(input, 2, output);
}

bool read_uint16(bdByteBufferReader* input, uint16_t* output)
{
    if (!read_data_type(input, BD_BB_UNSIGNED_INTEGER16_TYPE)) return false;
    return read(input, 2, output);
}

bool read_int32(bdByteBufferReader* input, int32_t* output)
{
    if (!read_data_type(input, BD_BB_SIGNED_INTEGER32_TYPE)) return false;
    return read(input, 4, output);
}

bool read_uint32(bdByteBufferReader* input, uint32_t* output)
{
    if (!read_data_type(input, BD_BB_UNSIGNED_INTEGER32_TYPE)) return false;
    return read(input, 4, output);
}

bool read_int64(bdByteBufferReader* input, int64_t* output)
{
    if (!read_data_type(input, BD_BB_SIGNED_INTEGER64_TYPE)) return false;
    return read(input, 8, output);
}

bool read_uint64(bdByteBufferReader* input, uint64_t* output)
{
    if (!read_data_type(input, BD_BB_UNSIGNED_INTEGER64_TYPE)) return false;
    return read(input, 8, output);
}

bool read_string(bdByteBufferReader* input, std::string* output)
{
    if (!read_data_type(input, BD_BB_SIGNED_CHAR8_STRING_TYPE)) return false;

    while (input->buffer.at(input->current_byte) != 0x00)
    {
        output->push_back(input->buffer.at(input->current_byte));
        input->current_byte++;
    }

    input->current_byte++;  return true;
}

bool read_string(bdByteBufferReader* input, char* output, int maxlen)
{
    if (!read_data_type(input, BD_BB_SIGNED_CHAR8_STRING_TYPE)) return false;

    for (size_t i = 0; i < maxlen; i++)
    {
        if (input->buffer.at(input->current_byte) == 0x00) break;

        output[i] = input->buffer.at(input->current_byte);
        input->current_byte++;
    }

    while (input->buffer.at(input->current_byte) != 0x00)
    {
        input->current_byte++;
    }

    input->current_byte++;  return true;
}

bool read_blob(bdByteBufferReader* input, std::vector<unsigned char>* output, int* length)
{
    if (!read_data_type(input, BD_BB_BLOB_TYPE))
    {
        return false;
    }

    unsigned int size = 0;
    read_uint32(input, &size);

    //if(size == 0) return true; // data expectation is correct but blob is empty

    if (size) output->insert(output->begin(), input->buffer.begin() + input->current_byte, input->buffer.begin() + input->current_byte + size);
    if (length) *length = static_cast<int>(size);

    input->current_byte += size;  return true;
}

bool reader_skip(bdByteBufferReader* input, unsigned int bytes)
{
    input->current_byte += bytes;  return true;
}

bool reader_skip(bdByteBufferReader* input, bdByteBufferDataType type)
{
    if (type == BD_BB_SIGNED_CHAR8_TYPE || type == BD_BB_UNSIGNED_CHAR8_TYPE) input->current_byte += 1;
    else if (type == BD_BB_SIGNED_INTEGER16_TYPE || type == BD_BB_UNSIGNED_INTEGER16_TYPE) input->current_byte += 2;
    else if (type == BD_BB_SIGNED_INTEGER32_TYPE || type == BD_BB_UNSIGNED_INTEGER32_TYPE) input->current_byte += 4;
    else if (type == BD_BB_SIGNED_INTEGER64_TYPE || type == BD_BB_UNSIGNED_INTEGER64_TYPE) input->current_byte += 8;
    else return false;

    input->current_byte++; // DataType itself

    return true;
}
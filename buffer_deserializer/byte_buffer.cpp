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
    "none", "bool", "int8_t", "uint8_t", "wchar_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t", "", "", "float32", "float64", "", "string", "", "", "binary", "", "", "", "structed"
};

bdByteBufferReader::bdByteBufferReader() {}
bdByteBufferReader::~bdByteBufferReader(){}


bdByteBufferReader::bdByteBufferReader(std::vector<unsigned char> input)
{
    this->buffer = input;
}

bool bdByteBufferReader::read(unsigned int bytes, void* output)
{
    if (bytes + this->current_byte > this->buffer.size()) return false;

    memcpy(output, this->buffer.data() + this->current_byte, bytes);
    this->current_byte += bytes;

    return true;
}

bool bdByteBufferReader::read_data_type(bdByteBufferDataType expected)
{
    if (!this->type_checked) return true;

    if (this->current_byte + 1 > this->buffer.size()) return false;

    if (this->buffer.at(this->current_byte) != expected) return false;

    this->current_byte++;  return true;
}

bool bdByteBufferReader::read_int16(int16_t* output)
{
    if (!this->read_data_type(BD_BB_SIGNED_INTEGER16_TYPE)) return false;
    return this->read(2, output);
}

bool bdByteBufferReader::read_uint16(uint16_t* output)
{
    if (!this->read_data_type(BD_BB_UNSIGNED_INTEGER16_TYPE)) return false;
    return this->read(2, output);
}

bool bdByteBufferReader::read_int32(int32_t* output)
{
    if (!this->read_data_type(BD_BB_SIGNED_INTEGER32_TYPE)) return false;
    return this->read(4, output);
}

bool bdByteBufferReader::read_uint32(uint32_t* output)
{
    if (!this->read_data_type(BD_BB_UNSIGNED_INTEGER32_TYPE)) return false;
    return this->read(4, output);
}

bool bdByteBufferReader::read_int64(int64_t* output)
{
    if (!this->read_data_type(BD_BB_SIGNED_INTEGER64_TYPE)) return false;
    return this->read(8, output);
}

bool bdByteBufferReader::read_uint64(uint64_t* output)
{
    if (!this->read_data_type(BD_BB_UNSIGNED_INTEGER64_TYPE)) return false;
    return this->read(8, output);
}

bool bdByteBufferReader::read_string(std::string* output)
{
    if (!this->read_data_type(BD_BB_SIGNED_CHAR8_STRING_TYPE)) return false;

    while (this->buffer.at(this->current_byte) != 0x00)
    {
        output->push_back(this->buffer.at(this->current_byte));
        this->current_byte++;
    }

    this->current_byte++;  return true;
}

bool bdByteBufferReader::read_string(char* output, int maxlen)
{
    if (!this->read_data_type(BD_BB_SIGNED_CHAR8_STRING_TYPE)) return false;

    for (size_t i = 0; i < maxlen; i++)
    {
        if (this->buffer.at(this->current_byte) == 0x00) break;

        output[i] = this->buffer.at(this->current_byte);
        this->current_byte++;
    }

    while (this->buffer.at(this->current_byte) != 0x00)
    {
        this->current_byte++;
    }

    this->current_byte++;  return true;
}

bool bdByteBufferReader::read_blob(std::vector<unsigned char>* output, int* length)
{
    if (!this->read_data_type(BD_BB_BLOB_TYPE))
    {
        return false;
    }

    unsigned int size = 0;
    this->read_uint32(&size);

    if (size) output->insert(output->begin(), this->buffer.begin() + this->current_byte, this->buffer.begin() + this->current_byte + size);
    if (length) *length = static_cast<int>(size);

    this->current_byte += size;  return true;
}

bool bdByteBufferReader::read_structed_data(std::vector<unsigned char>* output, int* length)
{
    if (!this->read_data_type(BD_BB_STRUCTURED_DATA_TYPE))
    {
        return false;
    }

    unsigned int size = 0;
    this->read_uint32(&size);

    if (size) output->insert(output->begin(), this->buffer.begin() + this->current_byte, this->buffer.begin() + this->current_byte + size);
    if (length) *length = static_cast<int>(size);

    this->current_byte += size;  return true;
}

bool bdByteBufferReader::reader_skip(unsigned int bytes)
{
    this->current_byte += bytes;  return true;
}

bool bdByteBufferReader::reader_skip(bdByteBufferDataType type, bool type_included)
{
    if (type == BD_BB_SIGNED_CHAR8_TYPE || type == BD_BB_UNSIGNED_CHAR8_TYPE) this->current_byte += 1;
    else if (type == BD_BB_SIGNED_INTEGER16_TYPE || type == BD_BB_UNSIGNED_INTEGER16_TYPE) this->current_byte += 2;
    else if (type == BD_BB_SIGNED_INTEGER32_TYPE || type == BD_BB_UNSIGNED_INTEGER32_TYPE) this->current_byte += 4;
    else if (type == BD_BB_SIGNED_INTEGER64_TYPE || type == BD_BB_UNSIGNED_INTEGER64_TYPE) this->current_byte += 8;
    else return false;

    if (type_included/*this->type_checked*/) this->current_byte++;

    return true;
}
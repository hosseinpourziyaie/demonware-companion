/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : byteBuffer structure Discovery
** - Description : automated code to read and discover data types in buffer  
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-08-25     | Ended on : 2022-08-28
** - Note        : new code shows data values in front of struct members as comments
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/

#include "byte_buffer.h"
#include "utilities.h"
#include <format>

unsigned int Discovery_getDataTypeSize(char type)
{
    switch (type)
    {
    case BD_BB_BOOL_TYPE: case BD_BB_SIGNED_CHAR8_TYPE: case BD_BB_UNSIGNED_CHAR8_TYPE:
        return 1;
    case BD_BB_SIGNED_INTEGER16_TYPE: case BD_BB_UNSIGNED_INTEGER16_TYPE:
        return 2;
    case BD_BB_SIGNED_INTEGER32_TYPE: case BD_BB_UNSIGNED_INTEGER32_TYPE:
        return 4;
    case BD_BB_SIGNED_INTEGER64_TYPE: case BD_BB_UNSIGNED_INTEGER64_TYPE:
        return 8;
    default:
        return 0; // corrupted or unknown data type
    }
}

void ByteBuffer_StructureDiscovery(const char* file)
{
    auto reader = new bdByteBufferReader{ read_binary_file(file), 0 }; std::string result("struct bdByteBufferDiscovery\n{\n"); int data_read_count = 0;

    while (reader->current_byte < reader->buffer.size())
    {
        char data_type; if (!read(reader, 1, &data_type)) break;

        if (data_type == BD_BB_BLOB_TYPE)
        {
            reader->current_byte--;
            data_read_count++;

            std::vector<unsigned char> binary_data; int length;
            read_blob(reader, &binary_data, &length);
            std::string binary_data_str(binary_data.begin(), binary_data.end());

            result.append(std::format("    char m_data{}[{}]; // {} (blob)\n", data_read_count, length, binary_data_str));
        }
        else if (data_type == BD_BB_SIGNED_CHAR8_STRING_TYPE)
        {
            reader->current_byte--;
            data_read_count++;

            std::string string_data;
            read_string(reader, &string_data);

            result.append(std::format("    std::string str{}; // {}\n", data_read_count, string_data));
        }
        else
        {
            int32_t size = Discovery_getDataTypeSize(data_type); if (size == 0) break;
            
            data_read_count++; std::string value_str;

            int64_t value; // why im so lazy to seperate type handles?
            read(reader, size, &value);

            if (size == 4) value_str = std::to_string((int)value);
            else if (size == 2) value_str = std::to_string((short)value);
            else if (size == 1) value_str = std::to_string((char)value);
            else value_str = std::to_string(value);

            result.append(std::format("    {} entry{}; // {}\n", bdByteBufferDataTypeNames[data_type], data_read_count, value_str));
        }
    }
    result.append("}");

    std::cout << result << std::endl << std::endl << "----------------------------------------------------------------------------------------------------------------" << std::endl;
}

//void ByteBuffer_StructureDiscovery(const char* file)
//{
//    auto reader = new bdByteBufferReader{ read_binary_file(file), 0 }; std::string result;
//
//    while (reader->current_byte < reader->buffer.size())
//    {
//        char data_type; if (!read(reader, 1, &data_type)) break;
//
//        if (data_type == BD_BB_BLOB_TYPE)
//        {
//            unsigned int size;
//            read_uint32(reader, &size);
//
//            reader->current_byte += size;
//            result.append("binary _ ");
//        }
//        else if (data_type == BD_BB_SIGNED_CHAR8_STRING_TYPE)
//        {
//            while (reader->buffer.at(reader->current_byte) != 0x00)
//            {
//                reader->current_byte++;
//            }
//
//            reader->current_byte++; // NULL TERMINATOR ITSELF
//            result.append("string _ ");
//        }
//        else
//        {
//            if (!Discovery_getDataTypeSize(data_type)) break;
//
//            if (!reader_skip(reader, Discovery_getDataTypeSize(data_type))) break;
//
//            result.append(bdByteBufferDataTypeNames[data_type]); result.append(" _ ");
//        }
//    }
//
//    if (result.length() > 3) result.erase(result.length() - 3, 3);
//
//    std::cout << result << std::endl << std::endl << "----------------------------------------------------------------------------------------------------------------" << std::endl;
//}
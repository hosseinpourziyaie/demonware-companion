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

//#define DUMP_BINARY_DATA

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

std::string Discovery_getDataValueString(char type, uint64_t value)
{
    switch (type)
    {
    case BD_BB_BOOL_TYPE:
        return std::to_string((bool)value); break;
    case BD_BB_SIGNED_CHAR8_TYPE:
        return std::to_string((char)value); break;
    case BD_BB_UNSIGNED_CHAR8_TYPE:
        return std::to_string((unsigned char)value); break;
    case BD_BB_SIGNED_INTEGER16_TYPE:
        return std::to_string((short)value); break;
    case BD_BB_UNSIGNED_INTEGER16_TYPE:
        return std::to_string((unsigned short)value); break;
    case BD_BB_SIGNED_INTEGER32_TYPE:
        return std::to_string((int)value); break;
    case BD_BB_UNSIGNED_INTEGER32_TYPE:
        return std::to_string((unsigned int)value); break;
    case BD_BB_SIGNED_INTEGER64_TYPE:
        return std::to_string((long long)value); break;
    case BD_BB_UNSIGNED_INTEGER64_TYPE:
        return std::to_string((unsigned long long)value); break;
    default:
        return std::to_string(value); break;
    }
}

void ByteBuffer_StructureDiscovery(const char* input_file)
{
    auto reader = new bdByteBufferReader(read_binary_file(input_file)); std::string result("struct bdByteBufferDiscovery\n{\n"); int data_read_count = 0;
    std::string file_name = get_file_name_from_path(input_file);

    while (reader->current_byte < reader->buffer.size())
    {
        char data_type; if (!reader->read(1, &data_type)) break;

        data_read_count++;

        if (data_type == BD_BB_BLOB_TYPE)
        {
            reader->current_byte--;

            std::vector<unsigned char> binary_data; int length;
            reader->read_blob(&binary_data, &length);

#ifdef DUMP_BINARY_DATA
            std::string dump_file = std::format("bytebuffer_dumps\\{}_blob{:x}", file_name, reader->current_byte - length);
            write_binary_file(binary_data, dump_file);

            result.append(std::format("    char entry{}[{}]; // dumped to \"{}\" (blob)\n", data_read_count, length, dump_file));
#else
            std::string binary_data_str(binary_data.begin(), binary_data.end());
            result.append(std::format("    char entry{}[{}]; // {} (blob)\n", data_read_count, length, binary_data_str));
#endif
        }
        else if (data_type == BD_BB_SIGNED_CHAR8_STRING_TYPE)
        {
            reader->current_byte--;

            std::string string_data;
            reader->read_string(&string_data);

            result.append(std::format("    std::string entry{}; // {}\n", data_read_count, string_data));
        }
        else if (data_type == BD_BB_STRUCTURED_DATA_TYPE)
        {
            reader->current_byte--;

            std::vector<unsigned char> structed_data; int length;
            reader->read_structed_data(&structed_data, &length);

#ifdef DUMP_BINARY_DATA
            std::string dump_file = std::format("bytebuffer_dumps\\{}_struct{:x}", file_name, reader->current_byte - length);
            write_binary_file(structed_data, dump_file);

            result.append(std::format("    char entry{}[{}]; // dumped to \"{}\" (struct)\n", data_read_count, length, dump_file));
#else
            std::string structed_data_str(structed_data.begin(), structed_data.end());
            result.append(std::format("    char entry{}[{}]; // {} (struct)\n", data_read_count, length, structed_data_str));
#endif
        }
        else if (data_type > 100) // BD_BB_ARRAY_TYPE
        {
            uint32_t array_size; uint32_t num_elements;
            reader->read_uint32(&array_size);
            reader->type_checked = false;
            reader->read_uint32(&num_elements);
            reader->type_checked = true;

            result.append(std::format("    array entry{}[{}]; // type:{} total size:{}\n", data_read_count, num_elements, data_type - 100, array_size));

            if (array_size == 0 || num_elements == 0) continue; // empty array

            result.append("    {\n");
            uint8_t elem_size = array_size / num_elements;

            for (int i = 0; i < num_elements; i++)
            {
                int64_t array_element;
                reader->read(elem_size, &array_element);
                result.append(std::format("        {}\n", array_element));
            }
            result.append("    }\n");
        }
        else
        {
            int32_t size = Discovery_getDataTypeSize(data_type); if (size == 0) break;         
            uint64_t value{};  reader->read(size, &value);

            result.append(std::format("    {} entry{}; // {}\n", bdByteBufferDataTypeNames[data_type], data_read_count, Discovery_getDataValueString(data_type, value)));
        }
    }
    result.append("}");

    std::cout << result << std::endl << std::endl << "----------------------------------------------------------------------------------------------------------------" << std::endl;
}
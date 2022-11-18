/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : byteBuffer Deserializer Main
** - Description : starting point to application
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-08-25     | Ended on : 2022-08-28
** - Note        : This console application has a creative drag n drop feature
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/

#include "byte_buffer.h"
#include "utilities.h"
#include "discovery.h"
#include <conio.h>

struct MarketplaceInventoryItem
{
    uint64_t m_userID;
    char m_accountType[16];
    uint32_t m_itemId;
    uint32_t m_itemQuantity;
    uint32_t m_itemXp;
    char m_itemData[64];
    uint32_t m_expireDateTime;
    int64_t m_expiryDuration;
    uint16_t m_collisionField;
    uint32_t m_modDateTime;
    uint8_t m_customSourceType;
}; // size: 128 

void bdMarketplaceInventory(const char* file/*, MarketplaceInventoryItem* output*/)
{
    auto reader = new bdByteBufferReader(read_binary_file(file));

    while (reader->current_byte < reader->buffer.size())
    {
        //auto output = new MarketplaceInventoryItem{};
        MarketplaceInventoryItem output = {};

        reader->read_uint64(&output.m_userID);
        reader->read_string(output.m_accountType, 16);
        std::cout << "m_userID: " << output.m_userID << "(" << output.m_accountType << ")" << std::endl;

        reader->read_uint32(&output.m_itemId);
        reader->read_uint32(&output.m_itemQuantity);
        reader->read_uint32(&output.m_itemXp);
        std::cout << "m_itemId: " << output.m_itemId << "(0x" << std::hex << std::uppercase << output.m_itemId << std::dec << ")" << " --  m_itemQuantity: " << output.m_itemQuantity << " --  m_itemXp: " << output.m_itemXp << std::endl;

        std::vector<unsigned char> m_itemData; int length;
        reader->read_blob(&m_itemData, &length);
        if(length)std::cout << "m_itemData: 0x" << std::hex << static_cast<int>(m_itemData.at(0)) << std::dec << std::endl;

        reader->read_uint32(&output.m_expireDateTime);
        reader->read_int64(&output.m_expiryDuration);
        std::cout << "m_expireDateTime: " << output.m_expireDateTime << " --  m_expiryDuration: " << output.m_expiryDuration << std::endl;

        reader->read_uint16(&output.m_collisionField);
        reader->read_uint32(&output.m_modDateTime);
        std::cout << "m_collisionField: " << output.m_collisionField << " --  m_modDateTime: " << output.m_modDateTime << std::endl;
        std::cout << "m_modDateTime: " << convert_unix_time(output.m_modDateTime) << std::endl;


        std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}

void bdMarketingComms(const char* file)
{
    auto reader = new bdByteBufferReader(read_binary_file(file));

    while (reader->current_byte < reader->buffer.size())
    {
        unsigned __int64 m_messageID; std::string m_languageCode;
        reader->read_uint64(&m_messageID);
        std::cout << "m_messageID: " << m_messageID << std::endl;

        reader->read_string(&m_languageCode);
        std::cout << "m_languageCode: <" << m_languageCode << ">" << std::endl;

        std::vector<unsigned char> m_content; int length;
        reader->read_blob(&m_content, &length);
        std::string m_content_str(m_content.begin(), m_content.end());
        std::cout << "m_content: " << m_content_str << std::endl;

        std::vector<unsigned char> m_metadata;
        reader->read_blob(&m_metadata, &length);
        std::string m_metadata_str(m_metadata.begin(), m_metadata.end());
        std::cout << "m_metadata: " << m_metadata_str << std::endl;

        std::cout << std::endl << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}

void ShowProgramOptions(char* file)
{
    std::cout << "Working sample: \"" << file << "\"" << std::endl << std::endl << "  Please choose desired function by inputing its specific number : " << std::endl << std::endl;
    std::cout << "  1- perform data structure Discovery" << std::endl;
    std::cout << "  2- deserialize bdMarketingComms buffer" << std::endl;
    std::cout << "  3- deserialize bdMarketplaceInventory buffer" << std::endl;

    int input;
    while (!(std::cin >> input) || (input < 1 || 4 < input)) {  //check the Input format for integer the right way
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.  Re-enter the number: ";
    };

    std::cout << std::endl << "----------------------------------------------------------------------------------------------------------------" << std::endl;

    switch (input)
    {
    case 1:
        ByteBuffer_StructureDiscovery(file);
        break;
    case 3:
        bdMarketplaceInventory(file);
        break;
    case 2:
        bdMarketingComms(file);
        break;
    default:
        std::cout << "Invalid FunctionID!" << std::endl;
        break;
    }
}

#include <stdlib.h>
int main(int argc, char* argv[])
{
    if (argc == 1) // Nothing inputed; Make Drag and Drop Console
    {
        std::cout << "Demonware Companion: ByteBuffer Tool (by Hosseinpourziyaie)" << std::endl << std::endl << "Help: Drag and Drop Demonware ByteBuffer Binary File on console to start!" << std::endl;

        std::string file_path;

        while (int ch = _getch())
        {
            if (ch == '\"') {
                while ((ch = _getch()) != '\"')
                    file_path += ch;
            }
            else {
                file_path += ch;

                while (_kbhit())
                    file_path += _getch();
            }

            if (is_file_exists(file_path)) break;
            else file_path.clear();
        }

        std::cout << std::endl;

        ShowProgramOptions(file_path.data());
    }
    else if (argc == 2)
    {
        if (is_file_exists(argv[1])) ShowProgramOptions(argv[1]);
        else std::cout << "Demonware Companion: ByteBuffer Tool" << std::endl << std::endl << "|--- Invalid input file(404 Not Found) ---|" << std::endl;
    }
    else
        std::cout << "Demonware Companion: ByteBuffer Tool" << std::endl << std::endl << "|--- Please Input Files one at a time ---|" << std::endl;

    system("pause"); // press any key to Exit ...
}
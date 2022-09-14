/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : Common Utilities
** - Description : set of usefull functions gathered in pne place
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-08-25     | Ended on : 2022-08-28
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/

#include "utilities.h"

std::string convert_unix_time(uint32_t timestamp)
{
    std::time_t temp = timestamp;
    std::tm* t = std::gmtime(&temp);
    std::stringstream ss;
    ss << std::put_time(t, "%Y-%m-%d %I:%M:%S %p");

    return ss.str();
}

bool is_file_exists(const std::string& filename)
{
    return _access_s(filename.c_str(), 0) == 0;
}

std::vector<unsigned char> read_binary_file(const std::string filename)
{
    // binary mode is only for switching off newline translation
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> vec;
    vec.reserve(file_size);
    vec.insert(vec.begin(),
        std::istream_iterator<unsigned char>(file),
        std::istream_iterator<unsigned char>());
    return (vec);
}
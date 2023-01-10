/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : Common Utilities
** - Description : set of usefull functions gathered in one place
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

bool is_file_exists(const std::string& file_path)
{
    return _access_s(file_path.c_str(), 0) == 0;
}

bool create_directory_if_not_exists(std::string directory)
{
    struct stat info;

    if (stat(directory.c_str(), &info) == -1) {
        if (_mkdir(directory.c_str()) == -1) return false;
    }

    return true;
}

std::string get_file_name_from_path(std::string path)
{
    const auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;

    return path.substr(pos + 1);
}

bool write_binary_file(std::vector<unsigned char> buffer, std::string file_path)
{
    const auto pos = file_path.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        if (create_directory_if_not_exists(file_path.substr(0, pos).c_str()) == false) return false;
    }

    std::ofstream stream(file_path, std::ios::binary | std::ios::out);
    if (!stream.is_open())  return false;

    stream.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    stream.close();

    return true;
}

std::vector<unsigned char> read_binary_file(const std::string file_path)
{
    std::ifstream file(file_path, std::ios::binary);
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